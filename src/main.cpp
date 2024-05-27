#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/Modify.hpp>

#include "tooltip.hpp"

using namespace geode::prelude;

struct EditorTooltips : Modify<EditorTooltips, EditorUI> {
  bool init(LevelEditorLayer* editorLayer) {
    if (!EditorUI::init(editorLayer)) {
      return false;
    }

    auto prev_layer_btn = this->getChildByIDRecursive("prev-layer-button");

    if (prev_layer_btn) {
      TooltipManager::get_instance()->add_tooltip(prev_layer_btn, "Go back a layer");
    }

    return true;
  }
};
