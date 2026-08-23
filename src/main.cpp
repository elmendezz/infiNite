#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/StatsManager.hpp>

using namespace geode::prelude;

class $modify(MyStatsMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("right-side-menu");
        if (!menu) menu = this->getChildByID("main-menu");

        auto spr = CircleButtonSprite::createWithSpriteFrameName("GJ_coinsIcon_001.png", 1.0f, CircleBaseColor::Green, CircleBaseSize::Medium);
        if (!spr) {
            spr = ButtonSprite::create("Stats");
        }

        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(MyStatsMenuLayer::onOpenStatsPopup)
        );
        btn->setID("stats-manager-button"_spr);

        if (menu) {
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onOpenStatsPopup(CCObject* sender) {
        auto popup = StatsPopup::create();
        popup->show();
    }
};

class StatsPopup : public Popup<SetStatsPopupDelegate> {
protected:
    TextInput* m_starsInput;
    TextInput* m_moonsInput;
    TextInput* m_diamondsInput;
    TextInput* m_orbsInput;
    TextInput* m_demonsInput;
    TextInput* m_coinsInput;
    TextInput* m_userCoinsInput;

    bool setup() override {
        this->setTitle("Stats Manager");

        auto gm = GameManager::sharedState();

        // Configurar UI de inputs
        auto layout = ColumnLayout::create()
            ->setAxis(Axis::Vertical)
            ->setGap(6.0f)
            ->setAxisAlignment(AxisAlignment::Center);

        auto contentNode = CCNode::create();
        contentNode->setContentSize({280.f, 200.f});
        contentNode->setLayout(layout);

        m_starsInput = createStatInput("Estrellas:", std::to_string(gm->getGameVariable("1")).c_str(), contentNode); // 1 = stars
        m_moonsInput = createStatInput("Moons:", std::to_string(gm->getGameVariable("28")).c_str(), contentNode);
        m_diamondsInput = createStatInput("Diamantes:", std::to_string(gm->getGameVariable("13")).c_str(), contentNode);
        m_orbsInput = createStatInput("Orbes:", std::to_string(gm->getGameVariable("14")).c_str(), contentNode);
        m_demonsInput = createStatInput("Demons:", std::to_string(gm->getGameVariable("5")).c_str(), contentNode);

        contentNode->updateLayout();
        m_mainLayer->addChildAtPosition(contentNode, Anchor::Center, ccp(0, 10));

        // Boton Aplicar
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
        rowNode->setContentSize({260.f, 30.f});
        
        auto label = CCLabelBMFont::create(labelText, "bigFont.fnt");
        label->setScale(0.4f);
        label->setAnchorPoint({0.f, 0.5f});
        label->setPosition({0.f, 15.f});
        rowNode->addChild(label);

        auto input = TextInput::create(100.f, "Cant.", "chatFont.fnt");
        input->setFilter("0123456789");
        input->setString(defaultVal);
        input->setAnchorPoint({1.f, 0.5f});
        input->setPosition({260.f, 15.f});
        rowNode->addChild(input);

        parent->addChild(rowNode);
        return input;
    }

    void onApply(CCObject* sender) {
        auto sm = StatsManager::sharedState();
        auto stats = geode::cocos::helper::getUnsavedStats(); // O modificar valores directamente

        // Guardar valores a GameManager / StatsManager
        if (m_starsInput) {
            int val = std::atoi(m_starsInput->getString().c_str());
            sm->setStat("1", val);
        }
        if (m_moonsInput) {
            int val = std::atoi(m_moonsInput->getString().c_str());
            sm->setStat("28", val);
        }
        if (m_diamondsInput) {
            int val = std::atoi(m_diamondsInput->getString().c_str());
            sm->setStat("13", val);
        }
        if (m_orbsInput) {
            int val = std::atoi(m_orbsInput->getString().c_str());
            sm->setStat("14", val);
        }
        if (m_demonsInput) {
            int val = std::atoi(m_demonsInput->getString().c_str());
            sm->setStat("5", val);
        }

        FLAlertLayer::create("Éxito", "¡Estadísticas aplicadas correctamente!", "OK")->show();
        this->onClose(sender);
    }

public:
    static StatsPopup* create() {
        auto ret = new StatsPopup();
        if (ret && ret->initAnchored(320.f, 260.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
