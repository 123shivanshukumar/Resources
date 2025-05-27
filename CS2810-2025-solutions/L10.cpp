#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <iomanip>

using namespace std;

// templatised custom class for complex numbers
template <typename Comp>
class Complex {
    public:
        // fields for the real and imaginary parts
        Comp real, imag;
        
        // constructor
        Complex(Comp r = 0, Comp i = 0) : real(r), imag(i) {}
    
        // overloaded + operator
        Complex<Comp> operator+(const Complex<Comp>& other) const {
            return Complex<Comp>(real + other.real, imag + other.imag);
        }
        
        // overloaded += operator
        Complex<Comp>& operator+=(const Complex<Comp>& other) {
            real += other.real;
            imag += other.imag;
            return *this;
        }
        
        // overloaded - operator
        Complex<Comp> operator-(const Complex<Comp>& other) const {
            return Complex<Comp>(real - other.real, imag - other.imag);
        }
        
        // overloaded * operator
        Complex<Comp> operator*(const Complex<Comp>& other) const {
            return Complex<Comp>(real * other.real - imag * other.imag, real * other.imag + imag * other.real);
        }
        
        // overloaded != operator
        bool operator!=(const Complex<Comp>& other) const {
            return real != other.real || imag != other.imag;
        }
        
        // overloaded == operator
        bool operator==(const Complex<Comp>& other) const {
            return real == other.real && imag == other.imag;
        }
        
        // overloaded << operator
        friend ostream& operator<<(ostream& os, const Complex<Comp>& c) {
            os << c.real << " " << c.imag;
            return os;
        }
};


// templatised multiplication class
template<typename Mult>
class Multiplication {
    public:
        // vector to store the coefficients
        vector<Mult> coefficients;

        // Constructor
        Multiplication(const vector<Mult>& coeffs) : coefficients(coeffs) {}

        // The overloaded * operator
        Multiplication<Mult> operator*(const Multiplication<Mult>& secondPolynomial) const{
            // this operator calls the recursive multiplication function that returns the vector containing the coefficients of the resultant polynomial
            return Multiplication<Mult> (polynomialMultiplication(coefficients, secondPolynomial.coefficients));
        }
        
        // function to print the coefficients
        void printCoefficients(long long int degree) const{
            bool noPrint = true;

            for (long long int i = 0; i < degree; i++) {
                cout << coefficients[i] << " ";
                noPrint = false;
            }

            if (noPrint)
                cout << "0";
        }
    
    private:
        // recursive function that implements the Karatsuba algorithm
        vector<Mult> polynomialMultiplication(const vector<Mult>& polynomial1, const vector<Mult>& polynomial2) const {
            long long int n = polynomial1.size();
            long long int m = polynomial2.size();
            
            // boundary condition
            if (n == 0 || m == 0)
                return vector<Mult>{};
            
            // if either of the polynomial is of size 1 then do a linear brute force multiplication
            if (n == 1 || m == 1) {
                vector<Mult> result(n + m - 1, Mult{});
                for (long long int i = 0; i < n; ++i)
                    for (long long int j = 0; j < m; ++j)
                        result[i + j] += polynomial1[i] * polynomial2[j];
                return result;
            }
            
            // the recursive calls on the left and right halves
            long long int mid = max(n, m) / 2;
        
            vector<Mult> a_low(polynomial1.begin(), polynomial1.begin() + min(mid, n));
            vector<Mult> a_high(polynomial1.begin() + min(mid, n), polynomial1.end());
            vector<Mult> b_low(polynomial2.begin(), polynomial2.begin() + min(mid, m));
            vector<Mult> b_high(polynomial2.begin() + min(mid, m), polynomial2.end());
        
            vector<Mult> P1 = polynomialMultiplication(a_low, b_low);
            vector<Mult> P2 = polynomialMultiplication(a_high, b_high);
            
            // combine the results
            vector<Mult> a_sum(max(a_low.size(), a_high.size()));
            for (long long int i = 0; i < a_sum.size(); i++)
                a_sum[i] = (i < a_low.size() ? a_low[i] : Mult{}) + (i < a_high.size() ? a_high[i] : Mult{});
        
            vector<Mult> b_sum(max(b_low.size(), b_high.size()));
            for (long long int i = 0; i < b_sum.size(); i++)
                b_sum[i] = (i < b_low.size() ? b_low[i] : Mult{}) + (i < b_high.size() ? b_high[i] : Mult{});
            
            // make the recursive call on the combined result
            vector<Mult> P3 = polynomialMultiplication(a_sum, b_sum);
        
            vector<Mult> result(n + m - 1, Mult{});
            
            // get the final result and return it
            for (long long int i = 0; i < P1.size(); i++)
                result[i] += P1[i];
        
            for (long long int i = 0; i < P2.size(); i++)
                result[i + 2 * mid] += P2[i];
        
            for (long long int i = 0; i < P3.size(); i++) {
                Mult middle = P3[i];
                if (i < P1.size()) middle = middle - P1[i];
                if (i < P2.size()) middle = middle - P2[i];
                result[i + mid] += middle;
            }
        
            return result;
        }
};


// templatised evaluation class
template<typename Eval>
class Evaluation {
    public:
        // vector to store the coefficients
        vector<Eval> coefficients;

        // Constructor
        Evaluation(const vector<Eval>& coeffs) : coefficients(coeffs) {}
        
        // function to perform the evaluation on integers and floats
        Eval evaluate(Eval xValue) {
            Eval result = coefficients.back();

            for (long long int index = coefficients.size() - 2; index >= 0; index--)
                result = result * xValue + coefficients[index];
            
            return result;
        }
        
        // function to perform the evaluation on strings
        Eval evaluateString(long long int xValue) {
            string result = "";

            for (long long int i = coefficients.size() - 1; i >= 0; i--) {
                if (!coefficients[i].empty()) {
                    if (i == 0) {
                        result = result + coefficients[i];
                    } else {
                        string concatenatedString = "";
                        for (long long int j = 0; j < pow(xValue, i); j++) {
                            concatenatedString += coefficients[i];
                        }
                        result = result + concatenatedString;
                    }
                }
            }

            return result;
        }

};


// templatised differentiation class
template<typename Diff>
class Differentiation {
    public:
        // vector to store the coefficients
        vector<Diff> coefficients;
        
        // Constructor
        Differentiation(const vector<Diff> coeffs) : coefficients(coeffs) {}
        
        // function to perform the differentiation operation
        Differentiation<Diff> differentiate() {
            long long int n = coefficients.size();
            if (n <= 1)
            return Differentiation<Diff>(vector<Diff>(1, static_cast<Diff>(0)));   // Derivative of constant is 0
        
            vector<Diff> result(n - 1);
            for (long long int i = 1; i < n; ++i) {
                result[i - 1] = coefficients[i] * static_cast<Diff>(i);
            }
        
            return Differentiation<Diff>(result);
        }
        
        // function to print the coefficients
        void printCoefficients() const{
            bool noPrint = true;

            for (long long int i = 0; i < coefficients.size(); i++) {
                cout << coefficients[i] << " ";
                noPrint = false;
            }

            if (noPrint)
                cout << "0";
        }
};


// main function
int main() {
    long long int queries;
    cin >> queries;
    
    for (long long int query = 1; query <= queries; query++) {
        long long int operation;
        cin >> operation;
        
        // if the operation is multiplication
        if (operation == 1) {
            string dataType;
            cin >> dataType;
            
            // create objects based on the data type of the values and call the respestive functions
            if (dataType == "integer") {
                vector<long long int> coefficients1, coefficients2;
                long long int degree1, degree2;

                cin >> degree1;

                for (long long int degree = 0; degree < degree1; degree++) {
                    long long int coefficient;
                    cin >> coefficient;
                    coefficients1.push_back(coefficient);
                }

                Multiplication<long long int> polynomial1(coefficients1);

                cin >> degree2;

                for (long long int degree = 0; degree < degree2; degree++) {
                    long long int coefficient;
                    cin >> coefficient;
                    coefficients2.push_back(coefficient);
                }

                Multiplication<long long int> polynomial2(coefficients2);

                Multiplication<long long int> resultPolynomial = polynomial1 * polynomial2;

                resultPolynomial.printCoefficients(degree1 + degree2 - 1);
                cout << endl;
            }

            else if (dataType == "float") {
                vector<long double> coefficients1, coefficients2;
                long long int degree1, degree2;

                cin >> degree1;

                for (long long int degree = 0; degree < degree1; degree++) {
                    long double coefficient;
                    cin >> coefficient;
                    coefficients1.push_back(coefficient);
                }

                Multiplication<long double> polynomial1(coefficients1);

                cin >> degree2;

                for (long long int degree = 0; degree < degree2; degree++) {
                    long double coefficient;
                    cin >> coefficient;
                    coefficients2.push_back(coefficient);
                }

                Multiplication<long double> polynomial2(coefficients2);

                Multiplication<long double> resultPolynomial = polynomial1 * polynomial2;
                
                cout << fixed << setprecision(6);
                resultPolynomial.printCoefficients(degree1 + degree2 - 1);
                cout << endl;
            }

            else if (dataType == "complex") {
                vector<Complex<long long int> > coefficients1, coefficients2;
                long long int degree1, degree2;

                cin >> degree1;

                for (long long int degree = 0; degree < degree1; degree++) {
                    long long int real, imaginary;
                    cin >> real >> imaginary;
                    Complex<long long int> number(real, imaginary);
                    coefficients1.push_back(number);
                }

                Multiplication<Complex<long long int> > polynomial1(coefficients1);

                cin >> degree2;

                for (long long int degree = 0; degree < degree2; degree++) {
                    long long int real, imaginary;
                    cin >> real >> imaginary;
                    Complex<long long int> number(real, imaginary);
                    coefficients2.push_back(number);
                }

                Multiplication<Complex<long long int> > polynomial2(coefficients2);

                Multiplication<Complex<long long int> > resultPolynomial = polynomial1 * polynomial2;
                
                resultPolynomial.printCoefficients(degree1 + degree2 - 1);
                cout << endl;
            }
        }


        // if the operation is evaluation
        else if (operation == 2) {
            string dataType;
            cin >> dataType;
            
            // create objects based on the data type of the values and call the respestive functions
            if (dataType == "integer") {
                vector<long long int> coefficients;
                
                long long int degree;
                cin >> degree;

                for (long long int currDegree = 0; currDegree < degree; currDegree++) {
                    long long int coefficient;
                    cin >> coefficient;
                    coefficients.push_back(coefficient);
                }

                Evaluation<long long int> polynomial(coefficients);

                long long int xValue;
                cin >> xValue;

                long long int result = polynomial.evaluate(xValue);

                cout << result << endl;
            }
            
            if (dataType == "float") {
                vector<long double> coefficients;
                
                long long int degree;
                cin >> degree;

                for (long long int currDegree = 0; currDegree < degree; currDegree++) {
                    long double coefficient;
                    cin >> coefficient;
                    coefficients.push_back(coefficient);
                }

                Evaluation<long double> polynomial(coefficients);

                long long int xValue;
                cin >> xValue;

                long double result = polynomial.evaluate(xValue);

                cout << fixed << setprecision(6) << result << endl;
            }

            if (dataType == "string") {
                vector<string> coefficients;
                
                long long int degree;
                cin >> degree;

                for (long long int currDegree = 0; currDegree < degree; currDegree++) {
                    string coefficient;
                    cin >> coefficient;
                    coefficients.push_back(coefficient);
                }

                Evaluation<string> polynomial(coefficients);

                long long int xValue;
                cin >> xValue;

                string result = polynomial.evaluateString(xValue);

                cout << result << endl;
            }
        }

        // if the operation is differentiation
        else if (operation == 3) {
            string dataType;
            cin >> dataType;
            
            // create objects based on the data type of the values and call the respestive functions
            if (dataType == "integer") {
                vector<long long int> coefficients;
                
                long long int degree;
                cin >> degree;

                for (long long int currDegree = 0; currDegree < degree; currDegree++) {
                    long long int coefficient;
                    cin >> coefficient;
                    coefficients.push_back(coefficient);
                }

                Differentiation<long long int> polynomial(coefficients);
                
                Differentiation<long long int> result = polynomial.differentiate();
                
                result.printCoefficients();

                cout << endl;
            }

            else if (dataType == "float") {
                vector<long double> coefficients;
                
                long long int degree;
                cin >> degree;

                for (long long int currDegree = 0; currDegree < degree; currDegree++) {
                    long double coefficient;
                    cin >> coefficient;
                    coefficients.push_back(coefficient);
                }

                Differentiation<long double> polynomial(coefficients);
                
                Differentiation<long double> result = polynomial.differentiate();
                
                cout << fixed << setprecision(6);
                result.printCoefficients();

                cout << endl;
            }
        }
    }

    return 0;
}