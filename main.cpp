//
// Created by spaceeye on 21.10.22.
//

#include <vector>
#include <random>
#include <cmath>
#include <chrono>
#include <iostream>

enum Types {
    T1,
    T2,
    T3,
    T4,
    T5,
    T6
};

enum Rotation {
    UP,
    LEFT,
    DOWN,
    RIGHT
};

struct WorldBlock1 {
    Types t;
};

struct WorldBlock2 {
    Types t1;
    Types t2;
    Types t3;
};

template<typename T>
struct Vector2 {
    T x;
    T y;
};

struct SerializedBlock {
    int relative_x;
    int relative_y;

    SerializedBlock()=default;
    SerializedBlock(int relative_x, int relative_y):
            relative_x(relative_x), relative_y(relative_y) {}

    //TODO i think i messed this up somehow
    inline Vector2<int> get_pos(Rotation rotation) {
        switch (rotation) {
            case Rotation::UP:    return Vector2<int>{relative_x, relative_y};
            case Rotation::LEFT:  return Vector2<int>{relative_y, -relative_x};
            case Rotation::DOWN:  return Vector2<int>{-relative_x, -relative_y};
            case Rotation::RIGHT: return Vector2<int>{-relative_y, relative_x};
            default: return Vector2<int>{relative_x, relative_y};
        }
    }
};

struct Organism {
public:
    int x = 0;
    int y = 0;

    std::vector<SerializedBlock> blocks;
    int min_x = INT32_MAX;
    int max_x = INT32_MIN;
    int min_y = INT32_MAX;
    int max_y = INT32_MIN;

    void make_uniform(int number) {
        int side = sqrt(number);

        blocks.reserve(number);

        for (int x = -side/2; x <= side/2; x++) {
            for (int y = -side/2; y <= side/2; y++) {
                blocks.emplace_back(x, y);
            }
        }
    }

    void make_random(int number, int radius, std::mt19937 & mt) {
        std::vector<int> x_positions;
        std::vector<int> y_positions;

        x_positions.reserve(number);
        y_positions.reserve(number);

        if (std::pow(radius*2, 2) < number) {
            radius = std::sqrt(number)/2+1;
        }

        for (int i = -radius; i < radius; i++) {
            x_positions.emplace_back(i);
            y_positions.emplace_back(i);
        }

        std::shuffle(x_positions.begin(), x_positions.end(), mt);
        std::shuffle(y_positions.begin(), y_positions.end(), mt);

        for (int i = 0; i < number; i++) {
            blocks.emplace_back(x_positions[i], y_positions[i]);
            if (x_positions[i] < min_x) {min_x = x_positions[i];}
            if (x_positions[i] > max_x) {max_x = x_positions[i];}

            if (y_positions[i] < min_y) {min_y = y_positions[i];}
            if (y_positions[i] > max_y) {max_y = y_positions[i];}
        }
    }
};

void create_organisms(std::vector<Organism> & organisms, int number, bool uniform, int radius, int width, int height) {
    for (int i = 0; i < number; i++) {
        int x = 0;
        int y = 0;

//        for ()
    }
}

using hrc = std::chrono::high_resolution_clock;
using ns = std::chrono::nanoseconds;
namespace ch = std::chrono;

struct s1 {
    int v;
};

int main() {
    std::vector<s1>  values1;
    std::vector<s1*> values2;

    int size = std::pow(10, 6);

    values1.resize(size, s1{0});

    for (int i = 0; i < size; i++) {
        values2.emplace_back(new s1{0});
    }

    auto t1 = hrc::now();
    auto t2 = hrc::now();

    uint64_t time = 0;

    for (int i = 0; i < size; i++) {
        t1 = hrc::now();
        values1[i].v += 1;
        t2 = hrc::now();
        time += ch::duration_cast<ns>(t2 - t1).count();
    }

    std::cout << "Linear access dense " << time / size << "\n";


    time = 0;

    for (int i = 0; i < size; i++) {
        t1 = hrc::now();
        values2[i]->v += 1;
        t2 = hrc::now();
        time += ch::duration_cast<ns>(t2 - t1).count();
    }

    std::cout << "Linear access pointers " << time / size << "\n";

    time = 0;

    std::mt19937 mt{42};

    for (int i = 0; i < size; i++) {
        int pos = std::uniform_int_distribution<int>(0, size)(mt);
        t1 = hrc::now();
        values1[pos].v += 1;
        t2 = hrc::now();
        time += ch::duration_cast<ns>(t2 - t1).count();
    }

    std::cout << "Random access dense " << time / size << "\n";

    time = 0;

    for (int i = 0; i < size; i++) {
        int pos = std::uniform_int_distribution<int>(0, size)(mt);
        t1 = hrc::now();
        values2[pos]->v += 1;
        t2 = hrc::now();
        time += ch::duration_cast<ns>(t2 - t1).count();
    }

    std::cout << "Random access pointers " << time / size << "\n";
}