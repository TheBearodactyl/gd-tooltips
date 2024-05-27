#pragma once

#include <Geode/Geode.hpp>
#include <Geode/cocos/CCDirector.h>
#include <Geode/cocos/label_nodes/CCLabelTTF.h>
#include <Geode/modify/CCLayer.hpp>
#include <Geode/modify/CCNode.hpp>
#include <climits>
#include <cocos2d.h>
#include <string>
#include <unordered_map>

using namespace geode::prelude;
using namespace cocos2d;

template <typename K, typename T> using UMap = std::unordered_map<K, T>;
using String = std::string;
using str = const std::string&;

class TooltipManager : public CCLayer {
private:
  UMap<CCNode*, String> tooltips;
  UMap<CCNode*, CCLabelTTF*> tooltip_labels;

  TooltipManager() {
    this->setTouchEnabled(true);

    CCDirector::sharedDirector()->getRunningScene()->addChild(this, INT_MAX);
  }

  void show_tooltip(CCNode* target_node, bool show) {
    auto it = tooltips.find(target_node);

    if (it != tooltips.end()) {
      auto tooltip = get_tooltip_label(target_node);

      if (show) {
        tooltip->setString(it->second.c_str());
        tooltip->setVisible(true);
      } else {
        tooltip->setVisible(false);
      }
    }
  }

  CCLabelTTF* get_tooltip_label(CCNode* target_node) {
    if (!tooltip_labels.count(target_node)) {
      auto label = CCLabelTTF::create("", "bigFont.fnt", 20);

      label->setVisible(false);
      label->setAnchorPoint({0.5f, 0});

      CCDirector::sharedDirector()->getRunningScene()->addChild(label, INT_MAX);

      tooltip_labels[target_node] = label;
    }

    auto tooltip_label = tooltip_labels[target_node];
    tooltip_label->setPosition({target_node->getPositionX(), target_node->getPositionY() + target_node->getContentSize().height / 2 + 10});

    return tooltip_label;
  }

  virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override { return true; }

  virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override { update_tooltips(touch->getLocation()); }

  virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override { update_tooltips(touch->getLocation()); }

public:
  static TooltipManager* get_instance() {
    static TooltipManager instance;

    return &instance;
  }

  void add_tooltip(CCNode* target_node, str tooltip_text) {
    if (target_node) {
      tooltips[target_node] = tooltip_text;
    }
  }

  void remove_tooltip(CCNode* target_node) {
    if (target_node) {
      tooltips.erase(target_node);
    }
  }

  void update_tooltips(const CCPoint& mouse_pos) {
    for (auto& [node, _] : tooltips) {
      if (node->boundingBox().containsPoint(mouse_pos)) {
        show_tooltip(node, true);
      } else {
        show_tooltip(node, false);
      }
    }
  }
};
