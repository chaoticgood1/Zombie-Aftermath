#include "GameScreenController.h"

GameScreenController::GameScreenController(GameScreen* gameScreenPtr) {
    this->gameScreenPtr = gameScreenPtr;
}

void GameScreenController::keyPressed(const int& keycode) {

}

void GameScreenController::keyReleased(const int& keycode) {

}

//------------------------ Left Mouse Pressed -----------------------------
void GameScreenController::leftMousePressed(const int& x, const int& y) {
    // Don't process any button if the panel button is pressed
    if (panelButtonPressed(x, y)) {
        return;
    }
}

const bool GameScreenController::panelButtonPressed(const int& x, const int& y) {
    PurchasePanel* purPanPtr = gameScreenPtr->rendererPtr->purchasePanPtr;
    purPanPtr->cursorX = x;
    purPanPtr->cursorY = y;

    int buttonId = purPanPtr->getButtonId(x, y);
    switch (buttonId) {
    case 0: purPanPtr->canName = "normalcannon";
        return true;
    case 1: purPanPtr->canName = "splashcannon";
        return true;
    case 2: purPanPtr->canName = "icecannon";
        return true;
    default:
        return false;
    }
}


//------------------------ Left Mouse Released ----------------------------
void GameScreenController::leftMouseReleased(const int& x, const int& y) {
    PurchasePanel* purPanPtr = gameScreenPtr->rendererPtr->purchasePanPtr;
    GameScreenUpdate* gameUpdater = gameScreenPtr->updatePtr;

    // If there is cannon name purchased
    if (purPanPtr->canName != "") {
        const Vector2i& tileNumber = getTileNumber(x, y);

        // Tile is available to be planted
        if (gameUpdater->tileStatus.at(tileNumber.y).at(tileNumber.x) == 0) {
            gameUpdater->createCannon(purPanPtr->canName, tileNumber.x, tileNumber.y);

            // Set value that indicates it has cannon
            gameUpdater->tileStatus.at(tileNumber.y).at(tileNumber.x) = 1;
            purPanPtr->canName = "";
        }

    }
    cancelCannonPlanting();
}

void GameScreenController::cancelCannonPlanting() {
    PurchasePanel* purPanPtr = gameScreenPtr->rendererPtr->purchasePanPtr;

    // Cancel the cursor and the cannon deployment
    purPanPtr->canName = "";
}


//------------------------ Right Mouse Pressed ----------------------------
void GameScreenController::rightMousePressed(const int& x, const int& y) {
    mousePrevPos.x = x;
    mousePrevPos.y = y;
    scrollScreen = true;
}


//------------------------ Right Mouse Released ---------------------------
void GameScreenController::rightMouseReleased(const int& x, const int& y) {
    scrollScreen = false;
}


//------------------------ Mouse move -------------------------------------
void GameScreenController::mouseMoved(const int& x, const int& y) {
    screenScrolling(x, y);
}

void GameScreenController::screenScrolling(const int& x, const int& y) {
    int scrollAmountX = x - mousePrevPos.x;
    int scrollAmountY = y - mousePrevPos.y;
    mousePrevPos.x = x;
    mousePrevPos.y = y;

    if (scrollScreen) {
        GameScreenUpdate* updatePtr = gameScreenPtr->updatePtr;
        Vector2f& mapPos = updatePtr->mapPos;

        // Limiting the screen scrolling to prevent seeing blank background
        if (mapPos.x <= 0) {
            mapPos.x += scrollAmountX;
            if (mapPos.x > 0) {
                mapPos.x = 0;
            }

            float& scrWidth = gameScreenPtr->gamePtr->width;
            if (mapPos.x < -scrWidth) {
                mapPos.x = -scrWidth;
            }
        }

        if (mapPos.y <= 0) {
            mapPos.y += scrollAmountY;
            if (mapPos.y > 0) {
                mapPos.y = 0;
            }

            float& scrHeight = gameScreenPtr->gamePtr->height;
            if (mapPos.y < -scrHeight) {
                mapPos.y = -scrHeight;
            }
        }
    }
}


//------------------------ Mouse Motion -----------------------------------
void GameScreenController::mouseMotion(const int& x, const int& y) {
    PurchasePanel* purPanPtr = gameScreenPtr->rendererPtr->purchasePanPtr;
    Vector2f& mapPos = gameScreenPtr->updatePtr->mapPos;

    // Place the cursor cannon in the area where it will be planted if the tile is available
    if (purPanPtr->canName != "") {
        // The value will be negative, I need positive value to ease the positioning
        int mapCorrectionX = abs( ((int) mapPos.x) % 32);
        int mapCorrectionY = abs( ((int) mapPos.y) % 32);

        // Corrects the tile coordinates in the current screen view where mouse cursor is currently at
        int tileX = (x + mapCorrectionX) / 32;
        int tileY = (y + mapCorrectionY) / 32;

        // Visual sprite cursor position correction
        purPanPtr->cursorX = (tileX * 32) - mapCorrectionX;
        purPanPtr->cursorY = (tileY * 32) - mapCorrectionY;
    }
}


//------------------------ Helper functions -----------------------------
const Vector2i& GameScreenController::getTileNumber(const int& screenX, const int& screenY) {
    Vector2f& mapPos = gameScreenPtr->updatePtr->mapPos;

    // Corrects the tile coordinates where mouse cursor at
    tileNumber.x = (abs( (int) mapPos.x ) + screenX) / 32;
    tileNumber.y = (abs( (int) mapPos.y ) + screenY) / 32;

    return tileNumber;
}

GameScreenController::~GameScreenController() {

}
