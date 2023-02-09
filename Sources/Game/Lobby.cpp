#include "Lobby.h"

void Game::Lobby::render(Engine::Graphics &g) {
    g.begin();
    g.clear(backgroundColour);

    auto x = Kore::System::windowWidth() / 2 - 700 / 2;
    auto y = 250;

    g.drawContainer(Kore::vec2{x - labelPadding, y + 200 - labelPadding}, Kore::vec2{700 + labelPadding * 2, 50 + labelPadding * 2}, 0xff809b80, containerColour);
    g.drawText(Kore::vec2{x, y}, "Vehicle Combat", headerColour, 100);
    g.drawText(Kore::vec2{x, y + 150}, "Username", labelColour, 30);
    g.drawText(Kore::vec2{x, y + 200}, nameField, fieldColour, 50);

    g.end();
}

bool invalidChar(char c) {
    return !(c >= 33 && c < 128);
}
void stripUnicode(std::string &str) {
    str.erase(remove_if(str.begin(), str.end(), invalidChar), str.end());
}

void Game::Lobby::update() {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string narrow = converter.to_bytes(Engine::Input::pressedKeys);
    stripUnicode(narrow);
    nameField += narrow;
    if (nameField.length() > maxNameLength) {
        nameField = nameField.substr(0, maxNameLength);
    }
    stripUnicode(nameField);

    if (Engine::Input::keysDown[Kore::KeyBackspace] && nameField.length() > 0 && std::chrono::system_clock::now() > keyTime) {
        nameField.pop_back();
        keyTime = std::chrono::system_clock::now() + std::chrono::milliseconds{fastKey ? 50 : 500};
        fastKey = true;
    }
    if (!Engine::Input::keysDown[Kore::KeyBackspace]) {
        fastKey = false;
        keyTime = std::chrono::system_clock::now();
    }

    if (Engine::Input::keysDown[Kore::KeyReturn]) {
        // TODO: Cleanup!
        auto playScene = new Game::Play(client, nameField);
        Engine::Core::getInstance().setScene(playScene);
    }

    Engine::Input::pressedKeys.clear();
}