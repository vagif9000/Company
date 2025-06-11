#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "test1.h"

class Company;

class Strategy {
public:
    virtual void apply(Company &c) = 0;
    virtual std::string description() const = 0;
    virtual ~Strategy() = default;
};

class Company {
    double revenue;
    double expenses;
    double employee_satisfaction;
    double product_quality;
    std::vector<Strategy *> improvement_plan;

public:
    Company(double _revenue, double _expenses, double _satisfaction, double _quality, const std::vector<Strategy *> &plan)
        : revenue(_revenue), expenses(_expenses), employee_satisfaction(_satisfaction), product_quality(_quality), improvement_plan(plan) {}

    double getRevenue() const { return revenue; }
    void setRevenue(double r) { revenue = r; }

    double getExpenses() const { return expenses; }
    void setExpenses(double e) { expenses = e; }

    double getSatisfaction() const { return employee_satisfaction; }
    void setSatisfaction(double s) { employee_satisfaction = s; }

    double getQuality() const { return product_quality; }
    void setQuality(double q) { product_quality = q; }

    void execute() {
        for (auto *s : improvement_plan) {
            s->apply(*this);
        }
    }

    Company(const Company &) = delete;
    Company &operator=(const Company &) = delete;

    ~Company() {
        for (auto *s : improvement_plan)
            delete s;
    }
};

class MarketingCampaign : public Strategy {
    int budget;
public:
    MarketingCampaign(int b) : budget(b) {}
    void apply(Company &c) override {
        double boost = std::min(budget * 0.05, 1.0);
        c.setRevenue(c.getRevenue() * (1 + boost));
        c.setExpenses(c.getExpenses() + budget);
    }
    std::string description() const override {
        return "Marketing Campaign with budget " + std::to_string(budget);
    }
};

class TrainingProgram : public Strategy {
    int sessions;
public:
    TrainingProgram(int s) : sessions(s) {}
    void apply(Company &c) override {
        double improvement = std::min(sessions * 0.1, 1.0);
        c.setSatisfaction(c.getSatisfaction() + improvement * 10);
        c.setQuality(c.getQuality() + improvement * 10);
    }
    std::string description() const override {
        return "Training Program with " + std::to_string(sessions) + " sessions";
    }
};

TEST(CompanyTest, Initialization) {
    std::vector<Strategy *> plan = {
        new MarketingCampaign(1000),
        new TrainingProgram(5)
    };

    Company c(50000, 20000, 70, 80, plan);

    ASSERT_EQ(c.getRevenue(), 50000);
    ASSERT_EQ(c.getExpenses(), 20000);
    ASSERT_EQ(c.getSatisfaction(), 70);
    ASSERT_EQ(c.getQuality(), 80);

    return true;
}

TEST(CompanyTest, StrategyEffect) {
    std::vector<Strategy *> plan = {
        new MarketingCampaign(1000),
        new TrainingProgram(5)
    };

    Company c(50000, 20000, 70, 80, plan);
    c.execute();

    ASSERT_TRUE(c.getRevenue() > 50000);
    ASSERT_TRUE(c.getExpenses() > 20000);
    ASSERT_TRUE(c.getSatisfaction() > 70);
    ASSERT_TRUE(c.getQuality() > 80);

    return true;
}

int main() {
    RUN_TEST(CompanyTest, Initialization);
    RUN_TEST(CompanyTest, StrategyEffect);
    return 0;
}
