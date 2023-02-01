#include "Lobby.h"

void Game::Lobby::render(Engine::Graphics &g) {
    g.begin();

    g.drawText(Kore::vec2{100, 100}, nameField);

    g.end();
}
void Game::Lobby::update() {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string narrow = converter.to_bytes(Engine::Input::pressedKeys);
    nameField += narrow;

    if (Engine::Input::keysDown[Kore::KeyBackspace] && nameField.length() > 0 && std::chrono::system_clock::now() > keyTime) {
        nameField.pop_back();
        keyTime = std::chrono::system_clock::now() + std::chrono::milliseconds{fastKey ? 50 : 500};
        fastKey = true;
    }
    if (!Engine::Input::keysDown[Kore::KeyBackspace]) {
        fastKey = false;
        keyTime = std::chrono::system_clock::now();
    }

    // Engine::Input::frameKeysDown.clear();
    Engine::Input::pressedKeys.clear();
}