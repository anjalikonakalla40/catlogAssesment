#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <iomanip>

using namespace std;

// Convert string number in given base to decimal (long long)
long long baseToDecimal(const string &value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = tolower(c) - 'a' + 10;
        else digit = 0;
        result = result * base + digit;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Reading JSON from stdin (paste JSON or redirect from file)
    string jsonInput, line;
    while (getline(cin, line)) {
        jsonInput += line;
    }

    // Remove spaces for easier parsing
    jsonInput.erase(remove(jsonInput.begin(), jsonInput.end(), ' '), jsonInput.end());

    // Extract n and k
    size_t posN = jsonInput.find("\"n\":");
    size_t posK = jsonInput.find("\"k\":");
    int n = stoi(jsonInput.substr(posN + 4, jsonInput.find(",", posN) - (posN + 4)));
    int k = stoi(jsonInput.substr(posK + 4, jsonInput.find("}", posK) - (posK + 4)));

    vector<pair<long long, long long>> points;

    // Parse each x entry: "x": {"base":"b","value":"v"}
    size_t pos = 0;
    while ((pos = jsonInput.find("\"", pos)) != string::npos) {
        size_t endKey = jsonInput.find("\"", pos + 1);
        string key = jsonInput.substr(pos + 1, endKey - pos - 1);

        // Skip "keys"
        if (key == "keys") {
            pos = endKey + 1;
            continue;
        }

        // Extract base
        size_t basePos = jsonInput.find("\"base\":\"", endKey);
        if (basePos == string::npos) break;
        basePos += 8;
        size_t baseEnd = jsonInput.find("\"", basePos);
        int base = stoi(jsonInput.substr(basePos, baseEnd - basePos));

        // Extract value
        size_t valPos = jsonInput.find("\"value\":\"", baseEnd);
        valPos += 9;
        size_t valEnd = jsonInput.find("\"", valPos);
        string value = jsonInput.substr(valPos, valEnd - valPos);

        long long x = stoll(key);
        long long y = baseToDecimal(value, base);

        points.push_back({x, y});
        pos = valEnd + 1;
    }

    // Take first k points
    vector<pair<long long, long long>> subset(points.begin(), points.begin() + k);

    // Lagrange interpolation to find P(0) (the constant term C)
    long double C = 0;
    for (int i = 0; i < k; i++) {
        long double term = subset[i].second;
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0.0 - subset[j].first) / (long double)(subset[i].first - subset[j].first);
            }
        }
        C += term;
    }

    cout << fixed << setprecision(0);
    cout << "Secret C = " << llround(C) << "\n";

    return 0;
}
