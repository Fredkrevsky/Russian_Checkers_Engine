#include "GameControllers.h"


void GameController::getData(MoveData& source) {
    assess = source.assess;
    memcpy(field, source.field, 64);
    type = source.type;
    vec = source.vec;
    x = source.x;
    y = source.y;
}

void GameController::setData(MoveData& dest) {
    dest.assess = assess;
    dest.x = x;
    dest.y = y;
    memcpy(dest.field, field, 64);
    memcpy(dest.oldfield, field, 64);
    dest.turn = turn;
    dest.type = type;
    dest.vec = vec;
}

GameController::GameController() {
    type = MOVE;
    x = y = vec = 0;
    turn = true;
    curr = 0;
    head = 0;
    assess = 0;
    BInit(field);

    MoveData temp;
    setData(temp);
    temp.coord[0] = 0;
    temp.coord[1] = 0;
    temp.coord[2] = 0;
    temp.coord[3] = 0;

    gameMoves.push_back(temp);
}

MOVE_RESULT GameController::PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2) {
    if (curr == head && !locked) {

        MoveData data;
        setData(data);
        data.coord[0] = x1;
        data.coord[1] = y1;
        data.coord[2] = x2;
        data.coord[3] = y2;

        MoveData temp = data;
        MOVE_RESULT result = engine.PlayerMove(data);
        if (result != INVALID_COORD) {
            memcpy(temp.field, data.field, 64);
            gameMoves.push_back(temp);
            getData(data);
            if (result == SUCCESS) {
                turn = !turn;
            }
            else if (result == WIN) {
                locked = true;
            }
            head++;
            curr++;
        }
        return result;
    }
    return MOVE_RESULT::INVALID_COORD;
}

MOVE_RESULT GameController::EngineMove(mytype depth) {
    if (!locked) {
        getCurr();

        MoveData data;
        setData(data);

        MoveData temp = data;
        MOVE_RESULT result = engine.EngineMove(data, depth);
        if (result == ONE_MORE || result == SUCCESS || result == WIN) {
            memcpy(temp.field, data.field, 64);
            memcpy(temp.coord, data.coord, 4);
            gameMoves.push_back(temp);
            getData(data);
            if (result == SUCCESS) {
                turn = !turn;
            }
            else if (result == WIN) {
                locked = true;
            }
            curr++;
            head++;
        }
        return result;
    }
    return INVALID_COORD;
}

void GameController::getPrev() {
    if (curr > 0) {
        curr--;
        MoveData temp = gameMoves[curr];
        memcpy(field, temp.field, 64);
        assess = temp.assess;
    }
}

void GameController::getNext() {
    if (curr < head) {
        curr++;
        MoveData temp = gameMoves[curr];
        memcpy(field, temp.field, 64);
        assess = temp.assess;
    }
}

void GameController::getCurr() {
    if (curr != head) {
        curr = head;
        MoveData temp = gameMoves[curr];
        memcpy(field, temp.field, 64);
        assess = temp.assess;
    }
}


void AnalysicsController::getData(MoveData& source) {
    assess = source.assess;
    memcpy(field, source.field, 64);
    type = source.type;
    vec = source.vec;
    x = source.x;
    y = source.y;
    x1 = source.coord[0];
    y1 = source.coord[1];
    x2 = source.coord[2];
    y2 = source.coord[3];
    comment = source.comment;
}

AnalysicsController::AnalysicsController() {

    x1 = x2 = y1 = y2 = 0;
    comment = FORCED;
    type = MOVE;
    x = y = vec = 0;
    turn = true;
    curr = 0;
    head = 0;
    assess = 0;
    BInit(field);

}

void AnalysicsController::evaluate(int index, int depth) {
    engine.evaluate(gameMoves[index], depth);
}

void AnalysicsController::setMoves(vector<MoveData>& tgameMoves) {
    gameMoves = tgameMoves;
}

void AnalysicsController::getPrev() {
    if (curr > 0) {
        curr--;
        getData(gameMoves[curr]);
    }
}

void AnalysicsController::getNext() {
    if (curr < gameMoves.size() - 1) {
        curr++;
        getData(gameMoves[curr]);
    }
}

void AnalysicsController::getCurr() {
    if (curr != head) {
        curr = head;
        getData(gameMoves[curr]);
    }
}

