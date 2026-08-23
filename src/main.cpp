#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/StatsManager.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class StatsPopup : public Popup<> {
protected:
    TextInput* m_starsInput = nullptr;
    TextInput* m_moonsInput = nullptr;
    TextInput* m_diamondsInput = nullptr;
    TextInput* m_orbsInput = nullptr;
    TextInput* m_demonsInput = nullptr;

    bool setup() override {
        this->setTitle("Stats Manager (infiNite)");

        auto gm = GameManager::sharedState();

        auto contentNode = CCNode::create();
        contentNode->setContentSize({280.f, 170.f});
        contentNode->setLayout(
            ColumnLayout::create()
                ->setAxis(Axis::Vertical)
                ->setGap(6.0f)
                ->setAxisAlignment(AxisAlignment::Center)
        );

        m_starsInput = createStatInput("Estrellas:", std::to_string(gm->getGameVariable("1")).c_str(), contentNode);
        m_moonsInput = createStatInput("Moons:", std::to_string(gm->getGameVariable("28")).c_str(), contentNode);
        m_diamondsInput = createStatInput("Diamantes:", std::to_string(gm->getGameVariable("13")).c_str(), contentNode);
        m_orbsInput = createStatInput("Orbes:", std::to_string(gm->getGameVariable("14")).c_str(), contentNode);
        m_demonsInput = createStatInput("Demons:", std::to_string(gm->getGameVariable("5")).c_str(), contentNode);

        contentNode->updateLayout();
        m_mainLayer->addChildAtPosition(contentNode, Anchor::Center, ccp(0, 10));

        auto applyBtnSpr = ButtonSprite::create("Aplicar", "goldFont.fnt", "GJ_button_01.png", 0.8f);
        auto applyBtn = CCMenuItemSpriteExtra::create(
            applyBtnSpr,
            this,
            menu_selector(StatsPopup::onApply)
        );

        auto buttonMenu = CCMenu::create();
        buttonMenu->addChild(applyBtn);
        m_mainLayer->addChildAtPosition(buttonMenu, Anchor::Bottom, ccp(0, 25));

        return true;
    }

    TextInput* createStatInput(const char* labelText, const char* defaultVal, CCNode* parent) {
        auto rowNode = CCNode::create();
        rowNode->setContentSize({260.f, 28.f});
        
        auto label = CCLabelBMFont::create(labelText, "bigFont.fnt");
        label->setScale(0.4f);
        label->setAnchorPoint({0.f, 0.5f});
        label->setPosition({0.f, 14.f});
        rowNode->addChild(label);

        auto input = TextInput::create(100.f, "0", "chatFont.fnt");
        input->setFilter("0123456789");
        input->setString(defaultVal);
        input->setAnchorPoint({1.f, 0.5f});
        input->setPosition({260.f, 14.f});
        rowNode->addChild(input);

        parent->addChild(rowNode);
        return input;
    }

    void onApply(CCObject* sender) {
        auto sm = StatsManager::sharedState();

        if (m_starsInput && !m_starsInput->getString().empty()) {
            int val = std::atoi(m_starsInput->getString().c_str());
            sm->setStat("1", val);
        }
        if (m_moonsInput && !m_moonsInput->getString().empty()) {
            int val = std::atoi(m_moonsInput->getString().c_str());
            sm->setStat("28", val);
        }
        if (m_diamondsInput && !m_diamondsInput->getString().empty()) {
            int val = std::atoi(m_diamondsInput->getString().c_str());
            sm->setStat("13", val);
        }
        if (m_orbsInput && !m_orbsInput->getString().empty()) {
            int val = std::atoi(m_orbsInput->getString().c_str());
            sm->setStat("14", val);
        }
        if (m_demonsInput && !m_demonsInput->getString().empty()) {
            int val = std::atoi(m_demonsInput->getString().c_str());
            sm->setStat("5", val);
        }

        FLAlertLayer::create("Éxito", "¡Estadísticas aplicadas correctamente!", "OK")->show();
        this->onClose(sender);
    }

public:
    static StatsPopup* create() {
        auto ret = new StatsPopup();
        if (ret && ret->initAnchored(320.f, 250.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

class $modify(MyStatsMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("right-side-menu");
        if (!menu) menu = this->getChildByID("main-menu");

        auto spr = CircleButtonSprite::createWithSpriteFrameName("GJ_coinsIcon_001.png", 1.0f, CircleBaseColor::Green, CircleBaseSize::Medium);
        if (!spr) {
            spr = ButtonSprite::create("infiNite");
        }

        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(MyStatsMenuLayer::onOpenStatsPopup)
        );
        btn->setID("infinite-button"_spr);

        if (menu) {
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onOpenStatsPopup(CCObject* sender) {
        auto popup = StatsPopup::create();
        if (popup) {
            popup->show();
        }
    }
};
