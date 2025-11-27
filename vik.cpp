#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

string trimLeadingZeros(const string &num) {
    size_t pos = num.find_first_not_of('0');
    if (pos != string::npos)
        return num.substr(pos);
    return "0";
}

int compare(const string &a, const string &b) {
    string num1 = trimLeadingZeros(a);
    string num2 = trimLeadingZeros(b);
    if (num1.size() != num2.size())
        return num1.size() > num2.size() ? 1 : -1;
    return num1.compare(num2);
}

string add(const string &a, const string &b) {
    string num1 = a;
    string num2 = b;
    reverse(num1.begin(), num1.end());
    reverse(num2.begin(), num2.end());

    string result;
    int carry = 0;
    size_t maxLength = max(num1.size(), num2.size());

    for (size_t index1 = 0; index1 < maxLength; ++index1) {
        int sum = carry;
        if (index1 < num1.size()) sum += num1[index1] - '0';
        if (index1 < num2.size()) sum += num2[index1] - '0';
        carry = sum / 10;
        result.push_back((sum % 10) + '0');
    }
    if (carry) result.push_back(carry + '0');

    reverse(result.begin(), result.end());
    return trimLeadingZeros(result);
}

string subtract(const string &a, const string &b) {
    if (compare(a, b) < 0) return "-" + subtract(b, a);

    string num1 = a;
    string num2 = b;
    reverse(num1.begin(), num1.end());
    reverse(num2.begin(), num2.end());

    string result;
    int borrow = 0;

    for (size_t index1 = 0; index1 < num1.size(); ++index1) {
        int diff = num1[index1] - '0' - borrow;
        if (index1 < num2.size()) diff -= (num2[index1] - '0');

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back(diff + '0');
    }

    reverse(result.begin(), result.end());
    return trimLeadingZeros(result);
}

string multiply(const string &a, const string &b) {
    string num1 = a;
    string num2 = b;
    int length1 = num1.size();
    int length2 = num2.size();

    vector<int> result(length1 + length2, 0);

    for (int index1 = length1 - 1; index1 >= 0; --index1) {
        for (int index2 = length2 - 1; index2 >= 0; --index2) {
            int product = (num1[index1] - '0') * (num2[index2] - '0');
            int sum = product + result[index1 + index2 + 1];

            result[index1 + index2 + 1] = sum % 10;
            result[index1 + index2] += sum / 10;
        }
    }

    string strResult;
    for (int digit : result) {
        if (!(strResult.empty() && digit == 0))
            strResult.push_back(digit + '0');
    }
    return strResult.empty() ? "0" : strResult;
}

string divide(const string &a, const string &b) {
    if (compare(b, "0") == 0) throw invalid_argument("Division by zero");
    if (compare(a, b) < 0) return "0";

    string dividend = a;
    string divisor = b;
    string result;

    while (compare(dividend, divisor) >= 0) {
        string tempDivisor = divisor;
        string quotientPart = "1";

        while (compare(add(tempDivisor, tempDivisor), dividend) <= 0) {
            tempDivisor = add(tempDivisor, tempDivisor);
            quotientPart = add(quotientPart, quotientPart);
        }

        dividend = subtract(dividend, tempDivisor);
        result = add(result, quotientPart);
    }
    return trimLeadingZeros(result);
}

int main() {
    string num1, num2;
    cin >> num1 >> num2;

    cout << add(num1, num2) << endl;
    cout << subtract(num1, num2) << endl;
    cout << multiply(num1, num2) << endl;

    try {
        cout << divide(num1, num2) << endl;
    } catch (invalid_argument &e) {
        cout << e.what() << endl;
    }

    int cmp = compare(num1, num2);
    if (cmp == 0)
        cout << "Equal" << endl;
    else if (cmp > 0)
        cout << "First is greater" << endl;
    else
        cout << "Second is greater" << endl;

    return 0;
}