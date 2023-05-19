#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <memory>
#include <iomanip>
using namespace std;

class Function {
public:
    virtual double evaluate(double x) const = 0;
    virtual unique_ptr<Function> antiderivative() const = 0;
    virtual void print(ostream& stream) const = 0;
    
    ~Function() = default;
    Function(const Function&) = delete;
    Function& operator=(const Function&) = delete;
protected:
    Function() = default;
};

class PolynomicalFunction : public Function {
private:
    vector<double> coefficients;
public:
    PolynomicalFunction(const vector<double>& coeffs) : coefficients(coeffs) {}

    double evaluate(double x) const override {
        double result = 0.0;
        for (int i = 0; i < coefficients.size(); i++) {
            result += coefficients[i] * pow(x, i);
        }
        return result;
    }

    unique_ptr<Function> antiderivative() const override {
        vector<double> newCoeffs(coefficients.size() + 1);
        for (int i = 0; i < coefficients.size(); i++) {
            newCoeffs[i + 1] = coefficients[i] / (i + 1);
        }
        return make_unique<PolynomicalFunction>(newCoeffs);
    }

    void print(ostream& stream) const override {
        for (int i = 0; i < coefficients.size(); i++) {
            if (i == 0) {
                cout << coefficients[i];
            }
            else {
                cout << " + " << coefficients[i] << "x^" << i;
            }
        }
        cout << endl;
    }
};

class Integrator {
public:
    virtual double integrate(const Function& f, double a, double b) = 0;
    virtual void print(ostream& stream) const = 0;
    
    ~Integrator() = default;
    Integrator(const Integrator&) = delete;
    Integrator& operator=(const Integrator&) = delete;
protected:
    Integrator() = default;
};
class AnalyticalIntegrator : public Integrator{
public:
    double integrate(const Function& f, double a, double b) override {
        auto p = f.antiderivative();
        return p->evaluate(b) - p->evaluate(a);
    }
    void print(ostream& stream) const override {
        cout << "Analytical";
    }
};
class RiemannIntegrator : public Integrator {
    double h;
public:
    RiemannIntegrator(double h=0.001) : h(h) {}
    double integrate(const Function& f, double a, double b) override {
        int n = (int)((b - a) / h);
        double sum = 0.0;
        for (int i = 0; i < n; i++) {
            double x1 = a + i * h;
            double x2 = x1 + h;
            double y1 = f.evaluate(x1);
            double y2 = f.evaluate(x2);
            sum += (x2 - x1) * ((y2 + y1) / 2);
        }
        return sum;
    }
    void print(ostream& stream) const override {
        cout << "Riemann Sum";
    }
};
void printTable(const vector<unique_ptr<Function>>& functions, const vector<unique_ptr<Integrator>>& itors, double a, double b) {
    for (int i = 0; i < functions.size(); ++i) {
        functions[i]->print(cout); 
        for (int j = 0; j < itors.size(); ++j) {
            cout << itors[j]->integrate(*functions[i],a,b) <<";";
        }
        cout << endl;
    }
}
int main() {
    vector<unique_ptr<Function>> functions;
    vector<unique_ptr<Integrator>> itors;
    vector<double> coeffs = { 2,0,0,4,0,0,0,5 };
    functions.push_back(make_unique<PolynomicalFunction>(coeffs));
    PolynomicalFunction fcn(coeffs);
    AnalyticalIntegrator integrator;
    itors.push_back(make_unique<AnalyticalIntegrator>());
    itors.push_back(make_unique<RiemannIntegrator>());
    printTable(functions, itors, 0.5, 1.5);
}