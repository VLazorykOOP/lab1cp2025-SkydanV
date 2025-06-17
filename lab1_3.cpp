#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

vector<pair<double, double>> table_U, table_T;
map<string, double> table_text;

double interpolate(const vector<pair<double, double>>& table, double x) {
    for (size_t i = 0; i < table.size() - 1; ++i) {
        if (x >= table[i].first && x <= table[i + 1].first) {
            double x0 = table[i].first, y0 = table[i].second;
            double x1 = table[i + 1].first, y1 = table[i + 1].second;
            return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
        }
    }
    return table.back().second;
}

double Km(double x, double y, double z);
double Rrz(double x, double y, double z);
double Qrz(double x, double y, double z);
double Srz(double x, double y, double z);

double Rrz2(double x, double y, double z);
double Qrz2(double x, double y, double z);
double Srz2(double x, double y, double z);

double Rrz3(double x, double y, double z);
double Qrz3(double x, double y, double z);
double Srz3(double x, double y, double z);

double T(double x);
double U(double x);

double fun(double x, double y, double z);


double fun(double x, double y, double z) {

    ifstream f1("dat_X_1_1.dat");
    ifstream f2("dat_X1_00.dat");
    ifstream f3("dat_X00_1.dat");

    if (f1.is_open() || f2.is_open() || f3.is_open()) {
        double res = x * Km(x, y, z) + y * Km(x, z, y) - z * Km(x, z, y);
        return res;
    }
    else {

        return 4.349 * x * x + 23.23 * y - 2.348 * x * y * z;
    }
}

double Km(double x, double y, double z) {
    return 83.1389 * Rrz2(x, y, z) + 4.838 * Rrz2(z, x, y);
}

double Rrz(double x, double y, double z) {
    if (x > y)
        return x * Qrz(y, z, x);
    else
        return y * Qrz(z, y, x);
}

double Qrz(double x, double y, double z) {
    if (abs(x) < 1)
        return x * Srz(x, y, z);
    else
        return y * Srz(z, y, x);
}

double Srz(double x, double y, double z) {
    if (z <= y)
        return y + 1.44 * z;
    else
        return y + 1.44 * Srz(x, y, z - 1);
}


double Rrz2(double x, double y, double z) {
    if (x > y)
        return x * Qrz2(y, z, x);
    else
        return y * Qrz2(z, y, x);
}

double Qrz2(double x, double y, double z) {
    if (abs(x) < 1)
        return x * Srz2(x, y, z);
    else
        return y * Srz2(z, y, x);
}

double Srz2(double x, double y, double z) {
    if (z <= y)
        return y + 1.44 * z;
    else
        return y + 1.44 * Srz2(x, y, z - 1);
}


double Rrz3(double x, double y, double z) {
    if (x > y)
        return x * Qrz3(y, z, x);
    else
        return y * Qrz3(z, y, x);
}

double Qrz3(double x, double y, double z) {
    if (abs(x) < 1)
        return x * Srz3(x, y, z);
    else
        return y * Srz3(z, y, x);
}

double Srz3(double x, double y, double z) {
    if (z <= y)
        return y + z;
    else
        return y + Srz3(x, y, z - 1);
}


double T(double x) {
    return interpolate(table_T, x);
}

double U(double x) {
    return interpolate(table_U, x);
}


void loadTableU(const string& filename) {
    table_U.clear();
    ifstream fin(filename);
    double x, u;
    while (fin >> x >> u) {
        table_U.push_back({ x, u });
    }
}

void loadTableT(const string& filename) {
    table_T.clear();
    ifstream fin(filename);
    double x, t;
    while (fin >> x >> t) {
        table_T.push_back({ x, t });
    }
}

void loadTableText(const string& filename) {
    table_text.clear();
    ifstream fin(filename);
    string text;
    double x;
    while (fin >> text >> x) {
        table_text[text] = x;
    }
}


void chooseFiles(double x) {
    if (abs(x) <= 1) {
        loadTableT("dat_X_1_1.dat");
        loadTableU("dat_X_1_1.dat");
    }
    else if (x < -1) {
        loadTableT("dat_X1_00.dat");
        loadTableU("dat_X1_00.dat");
    }
    else if (x > 1) {
        loadTableT("dat_X00_1.dat");
        loadTableU("dat_X00_1.dat");
    }
}

int main() {
    double x, y, z;
    cout << "Enter : x, y, z: ";
    cin >> x >> y >> z;

    chooseFiles(x);


    double result = fun(x, y, z);

    cout << "Result : fun(x,y,z) = " << result << endl;

    return 0;
}
