#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "test.h"

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

TEST(CompanyTest, ZeroBudgetAndSessions) {
    std::vector<Strategy *> plan = {
        new MarketingCampaign(0),
        new TrainingProgram(0)
    };

    Company c(50000, 20000, 70, 80, plan);
    c.execute();

    ASSERT_EQ(c.getRevenue(), 50000);
    ASSERT_EQ(c.getExpenses(), 20000);
    ASSERT_EQ(c.getSatisfaction(), 70);
    ASSERT_EQ(c.getQuality(), 80);

    return true;
}

TEST(CompanyTest, MaxEffectCap) {
    std::vector<Strategy *> plan = {
        new MarketingCampaign(100000),
        new TrainingProgram(20)
    };

    Company c(50000, 20000, 70, 80, plan);
    c.execute();

    ASSERT_EQ(c.getRevenue(), 50000 * 2);
    ASSERT_EQ(c.getExpenses(), 20000 + 100000);
    ASSERT_EQ(c.getSatisfaction(), 80);
    ASSERT_EQ(c.getQuality(), 90);

    return true;
}

TEST(CompanyTest, MultipleExecutions) {
    std::vector<Strategy *> plan = {
        new MarketingCampaign(500),
        new TrainingProgram(3)
    };

    Company c(50000, 20000, 70, 80, plan);
    c.execute();
    c.execute();

    ASSERT_TRUE(c.getRevenue() > 50000);
    ASSERT_TRUE(c.getExpenses() > 20000);
    ASSERT_TRUE(c.getSatisfaction() > 70);
    ASSERT_TRUE(c.getQuality() > 80);

    return true;
}

TEST(CompanyTest, NegativeValuesInput) {
    std::vector<Strategy *> plan = {
        new MarketingCampaign(100),
        new TrainingProgram(2)
    };

    Company c(-50000, -20000, -10, -5, plan);
    c.execute();

    ASSERT_TRUE(c.getRevenue() < 0);
    ASSERT_TRUE(c.getExpenses() < 0);
    ASSERT_TRUE(c.getSatisfaction() > -10);
    ASSERT_TRUE(c.getQuality() > -5);

    return true;
}

TEST(CompanyTest, NoStrategies) {
    std::vector<Strategy *> plan;
    Company c(10000, 5000, 60, 70, plan);

    c.execute();

    ASSERT_EQ(c.getRevenue(), 10000);
    ASSERT_EQ(c.getExpenses(), 5000);
    ASSERT_EQ(c.getSatisfaction(), 60);
    ASSERT_EQ(c.getQuality(), 70);

    return true;
}
TEST(CompanyTest, LargeBudgetSmallSessions) {
    std::vector<Strategy *> plan = {
        new MarketingCampaign(2000),
        new TrainingProgram(1)
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
    RUN_TEST(CompanyTest, ZeroBudgetAndSessions);
    RUN_TEST(CompanyTest, MaxEffectCap);
    RUN_TEST(CompanyTest, MultipleExecutions);
    RUN_TEST(CompanyTest, NegativeValuesInput);
    RUN_TEST(CompanyTest, NoStrategies);
    RUN_TEST(CompanyTest, LargeBudgetSmallSessions);
    return 0;
}
