#include <cmath>
#include <set>
#include <sstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <vector>
#include <map>

#include <gumbo.h>
#include <glog/logging.h>

#include <folly/Format.h>

#include "gooseFuncs.h"
#include "gumboUtils.h"
#include "stopwords.h"
#include "WhitespaceTokenizer.h"

using namespace std;

using folly::format;

namespace scivey {
namespace goosepp {

// all of this link density logic is straight from the scala version
bool isHighLinkDensity(const GumboNode *node, const string &nodeText) {
    auto allLinks = collectTag(GUMBO_TAG_A, node);
    if (allLinks.size() == 0) {
        return false;
    }
    WhitespaceTokenizer tokenizer;
    size_t linkTokenCount = 0;
    for (auto linkPtr: allLinks) {
        linkTokenCount += tokenizer.getTokenCount(cleanText(linkPtr));
    }
    double linkDivisor = ((double) linkTokenCount) / ((double) tokenizer.getTokenCount(nodeText));
    double score = linkDivisor * ((double) allLinks.size());
    if (score > 1.0) {
        return true;
    }
    return false;
}

bool isHighLinkDensity(const GumboNode *node) {
    return isHighLinkDensity(node, cleanText(node));
}

class TextNodeCollector {
protected:
    set<GumboTag> topNodeTags_ {
        GUMBO_TAG_P,
        GUMBO_TAG_PRE,
        GUMBO_TAG_TD
    };
public:
    TextNodeCollector(){}
    vector<const GumboNode*> collect(const GumboNode *root) {
        vector<const GumboNode*> nodesWithText;
        // auto nodesToCheck = collectTags(topNodeTags_, root);
        for (auto nodePtr: collectTags(topNodeTags_, root)) {
            string nodeText = cleanText(nodePtr);
            if (!isHighLinkDensity(nodePtr, nodeText) && countStopwords(nodeText) > 2) {
                nodesWithText.push_back(nodePtr);
            }
        }
        return nodesWithText;
    }
};

bool isOkToBoost(const GumboNode *node) {
    bool isOk = false;
    size_t stepsAway = 0;
    size_t minimumStopwordCount = 5;
    size_t maxStepsAway = 3;

    auto visitor = [&isOk, &stepsAway, minimumStopwordCount, maxStepsAway](const GumboNode* sibling, function<void()> escape) {
        if (sibling->type == GUMBO_NODE_ELEMENT && sibling->v.element.tag == GUMBO_TAG_P) {
            if (stepsAway >= maxStepsAway) {
                VLOG(4) << "next paragraph is too far away; boosting";
                isOk = false;
                escape();
                return;
            }
            auto siblingText = cleanText(sibling);
            if (countStopwords(siblingText) > minimumStopwordCount) {
                VLOG(4) << "sibling has a lot of stopwords; boosting";
                isOk = true;
                escape();
                return;
            }
            stepsAway++;
        }
    };
    walkSiblings(node, visitor);
    return isOk;
}

class NodeScorer {
    const GumboNode *root_;
    TextNodeCollector collector_;
    map<const GumboNode*, int> nodeScores_;
    map<const GumboNode*, size_t> nodeCounts_;
    set<const GumboNode*> parentNodes_;
    int topNodeScore_ {0};
    const GumboNode *topNode_ = nullptr;

public:
    NodeScorer(const GumboNode *root): root_(root){
        process();
    }

    int getNodeScore(const GumboNode *node) {
        if (nodeScores_.find(node) == nodeScores_.end()) {
            return 0;
        }
        return nodeScores_[node];
    }

    void updateNodeScore(const GumboNode *node, int score) {
        if (nodeScores_.find(node) == nodeScores_.end()) {
            nodeScores_[node] = 0;
        }
        nodeScores_[node] += score;
    }

    void incrNodeCount(const GumboNode *node) {
        if (nodeCounts_.find(node) == nodeCounts_.end()) {
            nodeCounts_[node] = 0;
        }
        nodeCounts_[node]++;
    }

    void updateTextyNode(const GumboNode *node, int boostScore) {
        if (node->parent != nullptr) {
            updateNodeScore(node->parent, boostScore);
            incrNodeCount(node->parent);
            parentNodes_.insert(node->parent);
        }
        if (node->parent->parent != nullptr) {
            updateNodeScore(node->parent->parent, boostScore / 2);
            incrNodeCount(node->parent->parent);
            parentNodes_.insert(node->parent->parent);
        }
    }

    void process() {
        if (topNode_ != nullptr) {
            return;
        }
        auto nodesWithText = collector_.collect(root_);
        double bottomNodesForNegativeScore = 0.25 * nodesWithText.size();
        double startingBoost = 1.0;
        size_t i = 0;

        for (auto node: nodesWithText) {
            double boostScore = 0;
            if (isOkToBoost(node)) {
                boostScore = ((1.0 / startingBoost) * 50);
                startingBoost += 1.0;
            }
            if (nodesWithText.size() > 15) {
                if ((nodesWithText.size() - i) <= bottomNodesForNegativeScore) {
                    double booster = bottomNodesForNegativeScore - ((double) nodesWithText.size() - i);
                    boostScore = -1.0 * pow(booster, 2.0);
                    double negScore = abs(boostScore);
                    if (negScore > 40) {
                        boostScore = 5.0;
                    }
                }
            }
            VLOG(4) << "boostScore [" << i << "] : " << boostScore;
            auto nodeText = cleanText(node);
            int upscore = countStopwords(nodeText) + ((int) boostScore);
            updateTextyNode(node, upscore);
            i++;
        }

        int topNodeScore = 0;
        const GumboNode *topNode = nullptr;
        for (auto node: parentNodes_) {
            int score = nodeScores_[node];
            if (score > topNodeScore) {
                topNode = node;
                topNodeScore = score;
            } else if (topNode == nullptr) {
                topNode = node;
            }
        }
        topNode_ = topNode;
        topNodeScore_ = topNodeScore;
    }
    const GumboNode* getTopNode() {
        return topNode_;
    }
    int getTopNodeScore() {
        return topNodeScore_;
    }
};


regex getBadClassesRegex() {
    const vector<string> badClasses = {
        "^side$|combx|retweet|mediaarticlerelated|menucontainer|",
        "navbar|storytopbar-bucket|utility-bar|inline-share-tools",
        "|comment|PopularQuestions|contact|foot|footer|Footer|footnote",
        "|cnn_strycaptiontxt|cnn_html_slideshow|cnn_strylftcntnt",
        "|^links$|meta$|shoutbox|sponsor",
        "|tags|socialnetworking|socialNetworking|cnnStryHghLght",
        "|cnn_stryspcvbx|^inset$|pagetools|post-attributes",
        "|welcome_form|contentTools2|the_answers",
        "|communitypromo|runaroundLeft|subscribe|vcard|articleheadings",
        "|date|^print$|popup|author-dropdown|tools|socialtools|byline",
        "|konafilter|KonaFilter|breadcrumbs|^fn$|wp-caption-text",
        "|legende|ajoutVideo|timestamp|js_replies"
    };
    ostringstream oss;
    for (auto &elem: badClasses) {
        oss << elem;
    }
    string reggie = oss.str();
    regex bad_re(reggie);
    return bad_re;
}


class NodeTextCleaner {
protected:
    regex badClasses_;
    set<GumboTag> badTagTypes_ {
        GUMBO_TAG_STYLE, GUMBO_TAG_SCRIPT, GUMBO_TAG_EM
    };

    bool isBadTextNode(const GumboNode *node) {
        if (node->type == GUMBO_NODE_ELEMENT) {
            if (badTagTypes_.find(node->v.element.tag) != badTagTypes_.end()) {
                return true;
            }
            auto idAttr = gumbo_get_attribute(&node->v.element.attributes, "id");
            if (idAttr != nullptr) {
                string ids = idAttr->value;
                if (regex_search(ids, badClasses_)) {
                    return true;
                }
            }
            auto classesAttr = gumbo_get_attribute(&node->v.element.attributes, "class");
            if (classesAttr != nullptr) {
                string classes = classesAttr->value;
                if (regex_search(classes, badClasses_)) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    NodeTextCleaner(){
        badClasses_ = getBadClassesRegex();
    }

    string getText(const GumboNode *node) {
        string result {""};
        if (node->type == GUMBO_NODE_TEXT) {
            result = node->v.text.text;
        } else if (node->type == GUMBO_NODE_ELEMENT) {
            if (!isBadTextNode(node)) {
                ostringstream output;
                const GumboVector *children = &node->v.element.children;
                for (size_t i = 0; i < children->length; ++i) {
                    const string text = getText(static_cast<GumboNode*>(children->data[i]));
                    if (i != 0 && !text.empty()) {
                        output << " ";
                    }
                    output << text;
                }
                result = output.str();
            }
        }
        return result;
    }

};

class ContentExtractor {
protected:
    NodeScorer scorer_;
    const GumboNode *root_;
public:
    ContentExtractor(const GumboNode *root): root_(root), scorer_(root) {
    }
    string extract() {
        auto topNode = scorer_.getTopNode();
        int topNodeScore = scorer_.getTopNodeScore();
        vector<const GumboNode*> goodNodes;
        {
            const GumboVector *children = &topNode->v.element.children;
            double thresholdScore = ((double) topNodeScore) * 0.08;
            for (size_t i = 0; i < children->length; ++i) {
                auto node = static_cast<const GumboNode*>(children->data[i]);
                if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag != GUMBO_TAG_P) {
                    if (isHighLinkDensity(node)) {
                        continue;
                    }
                    if (node->v.element.tag != GUMBO_TAG_TD) {
                        double nodeScore = (double) scorer_.getNodeScore(node);
                        if (nodeScore < thresholdScore) {
                            continue;
                        }
                    }
                }
                auto nodeText = cleanText(node);
                if (countStopwords(nodeText) < 3) {
                    continue;
                }
                goodNodes.push_back(node);
            }
        }
        NodeTextCleaner cleaner;
        ostringstream bodyStream;
        for (auto &elem: goodNodes) {
            bodyStream << cleaner.getText(elem);
        }
        return bodyStream.str();
    }
};

string getBestContent(const GumboNode *root) {
    NodeScorer scorer(root);
    auto topNode = scorer.getTopNode();
    int topNodeScore = scorer.getTopNodeScore();
    vector<const GumboNode*> goodNodes;
    {
        const GumboVector *children = &topNode->v.element.children;
        double thresholdScore = ((double) topNodeScore) * 0.08;
        for (size_t i = 0; i < children->length; ++i) {
            auto node = static_cast<const GumboNode*>(children->data[i]);
            if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag != GUMBO_TAG_P) {
                if (isHighLinkDensity(node)) {
                    continue;
                }
                if (node->v.element.tag != GUMBO_TAG_TD) {
                    double nodeScore = (double) scorer.getNodeScore(node);
                    if (nodeScore < thresholdScore) {
                        continue;
                    }
                }
            }
            auto nodeText = cleanText(node);
            if (countStopwords(nodeText) < 3) {
                continue;
            }
            goodNodes.push_back(node);
        }
    }
    NodeTextCleaner cleaner;
    ostringstream bodyStream;
    for (auto &elem: goodNodes) {
        bodyStream << cleaner.getText(elem);
    }
    return bodyStream.str();
}


string extractBody(const string &htmlText) {
    auto gumboed = gumbo_parse(htmlText.c_str());
    return getBestContent(gumboed->root);
}

} // goosepp
} // scivey
