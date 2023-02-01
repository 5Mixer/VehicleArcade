#include "Lobby.h"

void Game::Lobby::render(Engine::Graphics &g) {
    g.begin();

    g.drawText(Kore::vec2{100, 100}, nameField);

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

    Engine::Input::pressedKeys.clear();
}