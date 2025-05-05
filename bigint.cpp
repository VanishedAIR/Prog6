// Name: Vansh Patel
// Class (CECS 325-02)
// Project Name (Prog 6 - Big Integer)
// Due Date (05/08/25)
// I certify that this program is my own original work. I did not copy and part of this program from any other source.
// I further certify that I typed each and every line of code in this program.

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <iomanip>

using namespace std;

class BigInt
{
private:
    vector<char> v; //<- notice this is a vector of char, not int

public:
    BigInt()
    {
        v.push_back(0); // default value is 0
    }

    BigInt(int number)
    {
        if (number == 0)
        {
            v.push_back(0);
            return;
        }

        while (number > 0)
        {
            v.push_back(number % 10); // stores it in reverse, making operation easier
            number /= 10;
        }
    }

    BigInt(const string &str)
    {
        int i = str.length() - 1;
        while (i >= 0)
        {
            if (isdigit(str[i]))
            {
                v.push_back(str[i] - '0'); // convert using ascii value starting at 0 which is 48
            }
            i--;
        }

        if (v.empty())
        {
            v.push_back(0);
        }
    }

    BigInt operator+(const BigInt &b)
    {
        BigInt result; //<- result is a BigInt
        result.v.clear();

        int carry = 0;
        int i = 0;
        int j = 0;

        while (i < v.size() || j < b.v.size() || carry)
        {
            int sum = carry; // start calculation with carry in mind
            if (i < v.size())
                sum += v[i++];
            if (j < b.v.size())
                sum += b.v[j++]; // adds the digits

            result.v.push_back(sum % 10); // stores the sum in reverse
            carry = sum / 10;             // calculates the carry
        }

        return result;
    }

    BigInt operator-(const BigInt &b)
    {
        if (this->operator<(b))
            return BigInt(0);

        BigInt result;
        result.v.clear();

        int borrow = 0;
        int i = 0, j = 0;

        while (i < v.size())
        {
            int diff = v[i] - borrow;
            if (j < b.v.size())
                diff -= b.v[j++]; // subtracts the digits

            if (diff < 0) // if diff is negative
            {
                diff += 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }

            result.v.push_back(diff); // stores the difference in reverse
            i++;
        }

        // remove zeros
        while (result.v.size() > 1 && result.v[result.v.size() - 1] == 0)
        {
            result.v.pop_back();
        }

        return result;
    }

    BigInt operator*(const BigInt &b)
    {
        BigInt zero(0);
        BigInt one(1);

        BigInt bCopy = b;

        if (this->operator==(zero) || bCopy.operator==(zero))
            return BigInt(0);
        if (this->operator==(one))
            return bCopy;
        if (bCopy.operator==(one))
            return *this;

        if (b.v.size() > v.size()) // choose the smaller num
        {
            BigInt bCopy2 = b;
            return bCopy2.operator*(*this);
        }

        BigInt result(0);
        BigInt counter(0);
        BigInt temp = *this;

        while (counter.operator<(bCopy))
        {
            result = result + temp;
            counter = counter + one;
        }

        return result;
    }

    BigInt operator/(const BigInt &b)
    {
        // div is repeated subtraction
        BigInt result(0);
        BigInt one(1);
        BigInt temp = *this;

        while (temp.operator>=(b))
        {
            temp = temp - b;
            result = result + one;
        }

        return result;
    }

    BigInt operator%(const BigInt &b)
    {
        BigInt temp = *this;

        while (temp.operator>=(b))
        {
            temp = temp - b;
        }

        return temp; // returns the remainder
    }

    bool operator==(const BigInt &b)
    {
        if (v.size() != b.v.size()) // if the sizes are different then they are not equal
            return false;

        for (int i = 0; i < v.size(); i++) // checks each digit
        {
            if (v[i] != b.v[i]) // if any digit is not equal then they are not equal
                return false;
        }

        return true;
    }

    bool operator<(const BigInt &b)
    {
        if (v.size() < b.v.size()) // if v is smaller in size then it is smaller than b
            return true;
        if (v.size() > b.v.size()) // if v is larger in size then it is larger than b
            return false;

        for (int i = v.size() - 1; i >= 0; i--)
        {
            if (v[i] < b.v[i]) // a single digit of v is smaller than b then its smaller overall
                return true;
            if (v[i] > b.v[i])
                return false;
        }

        return false; // also return false if they are equal
    }

    bool operator>(const BigInt &b)
    {
        BigInt bCopy = b;
        return bCopy.operator<(*this); // if b is smaller than v then v is larger than b
    }

    bool operator<=(const BigInt &b)
    {
        return this->operator<(b) || this->operator==(b); // if v is smaller than b or they are equal then v is smaller than b or they are equal
    }

    bool operator>=(const BigInt &b)
    {
        return this->operator>(b) || this->operator==(b);
    }

    BigInt &operator++() // pre increment
    {
        *this = *this + BigInt(1);
        return *this;
    }

    BigInt operator++(int) // post increment
    {
        BigInt temp = *this;
        *this = *this + BigInt(1);
        return temp; // return the original value before incrementing
    }

    char operator[](int index) // index function
    {
        if (index >= 0 && index < (int)(v.size()))
        {
            return v[index];
        }
        return 0; // out of bounds
    }

    int size()
    {
        return v.size();
    }

    void print()
    {
        for (int i = v.size() - 1; i >= 0; i--) // prints in reverse since we stored in reverse
        {
            cout << (int)(v[i]); // convert to int
        }
    }

    // do tail recursion
    static BigInt fibo(const BigInt &n, BigInt a = BigInt(0), BigInt b = BigInt(1))
    {
        BigInt zero(0);
        BigInt one(1);

        BigInt nCopy = n;

        if (nCopy.operator==(zero))
            return a;
        if (nCopy.operator==(one))
            return b;
        BigInt nMinusOne = nCopy.operator-(one);
        return fibo(nMinusOne, b, a + b);
    }

    BigInt fibo() // need instance version for unit test call to fibo.fibo()
    {
        return BigInt::fibo(*this);
    }

    static BigInt fact(const BigInt &n)
    {
        BigInt one(1);
        BigInt nCopy = n;

        if (nCopy.operator<=(one))
            return one;
        BigInt nMinusOne = nCopy.operator-(one);
        return nCopy * fact(nMinusOne);
    }

    // need instance version for unit test call to fact.fact()
    BigInt fact()
    {
        return BigInt::fact(*this);
    }

    friend BigInt operator+(int a, const BigInt &b)
    {
        return BigInt(a) + b; // convert a to BigInt and add it to b (big int)
    }

    friend ostream &operator<<(ostream &out, const BigInt &b)
    {
        if (b.v.size() > 12)
        {
            int exponent = b.v.size() - 1;
            out << (int)(b.v[b.v.size() - 1]) << "."; // first digit.

            for (int i = 0; i < 6 && b.v.size() - 2 - i >= 0; i++)
            {
                out << (int)(b.v[b.v.size() - 2 - i]);
            }

            out << "e" << exponent;
        }
        else // not long enough
        {
            for (int i = b.v.size() - 1; i >= 0; i--)
            {
                out << (int)(b.v[i]);
            }
        }
        return out;
    }
};

void testUnit()
{
    int space = 10;
    cout << "\a\nTestUnit:\n"
         << flush;
    system("whoami");
    system("date");
    // initialize variables
    BigInt n1(25);
    BigInt s1("25");
    BigInt n2(1234);
    BigInt s2("1234");
    BigInt n3(n2);
    BigInt fibo(12345);
    BigInt fact(50);
    BigInt imax = INT_MAX;
    BigInt big("9223372036854775807");
    // display variables
    cout << "n1(int) :" << setw(space) << n1 << endl;
    cout << "s1(str) :" << setw(space) << s1 << endl;
    cout << "n2(int) :" << setw(space) << n2 << endl;
    cout << "s2(str) :" << setw(space) << s2 << endl;
    cout << "n3(n2) :" << setw(space) << n3 << endl;
    cout << "fibo(12345):" << setw(space) << fibo << endl;
    cout << "fact(50) :" << setw(space) << fact << endl;
    cout << "imax :" << setw(space) << imax << endl;
    cout << "big :" << setw(space) << big << endl;
    cout << "big.print(): ";
    big.print();
    cout << endl;
    cout << n2 << "/" << n1 << " = " << n2 / n1 << " rem " << n2 % n1 << endl;
    cout << "fibo(" << fibo << ") = " << fibo.fibo() << endl;
    cout << "fact(" << fact << ") = " << fact.fact() << endl;
    cout << "10 + n1 = " << 10 + n1 << endl;
    cout << "n1 + 10 = " << n1 + 10 << endl;
    cout << "(n1 == s1)? --> " << ((n1 == s1) ? "true" : "false") << endl;
    cout << "n1++ = ? --> before:" << n1++ << " after:" << n1 << endl;
    cout << "++s1 = ? --> before:" << ++s1 << " after:" << s1 << endl;
    cout << "s2 * big = ? --> " << s2 * big << endl;
    cout << "big * s2 = ? --> " << big * s2 << endl;
}

int main()
{
    testUnit();
    return 0;
}