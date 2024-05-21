#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"
extern WareHouse* backup;
using std::string;
using std::vector;



enum class ActionStatus{
    COMPLETED, ERROR
};

enum class CustomerType{
    Soldier, Civilian
};


class Customer;

class BaseAction{
    public:
        BaseAction();
        BaseAction(const BaseAction& other); // New Copy Constructor
        virtual ~BaseAction() = default;
        ActionStatus getStatus() const;
        string ActionStatusToString(ActionStatus status) const;
        virtual void act(WareHouse& wareHouse)=0;
        virtual string toString() const=0;
        virtual BaseAction* clone() const=0;

    protected:
        void complete();
        void error(string errorMsg);
        string getErrorMsg() const;

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public BaseAction {

    public:
        SimulateStep(int numOfSteps);
        SimulateStep(const SimulateStep &other); // New Copy Constructor
        void act(WareHouse &wareHouse) override;
        std::string toString() const override;
        SimulateStep *clone() const override;

    private:
        const int numOfSteps;
};

class AddOrder : public BaseAction {
    public:
        AddOrder(int id);
        AddOrder(const AddOrder &other); // New Copy Constructor
        void act(WareHouse &wareHouse) override;
        string toString() const override;
        AddOrder *clone() const override;
    private:
        const int customerId;
};


class AddCustomer : public BaseAction {
    public:
        AddCustomer(string customerName, string customerType, int distance, int maxOrders);
        AddCustomer(const AddCustomer &other); // New Copy Constructor
        void act(WareHouse &wareHouse) override;
        AddCustomer *clone() const override;
        string CustomerTypeToString(CustomerType customerType) const;
        string toString() const override;
    private:
        const string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;
};



class PrintOrderStatus : public BaseAction {
    public:
        PrintOrderStatus(int id);
        PrintOrderStatus(const PrintOrderStatus &other); // New Copy Constructor   
        void act(WareHouse &wareHouse) override;
        PrintOrderStatus *clone() const override;
        string toString() const override;
    private:
        const int orderId;
};

class PrintCustomerStatus: public BaseAction {
    public:
        PrintCustomerStatus(int customerId);
        PrintCustomerStatus(const PrintCustomerStatus &other); // New Copy Constructor
        void act(WareHouse &wareHouse) override;
        PrintCustomerStatus *clone() const override;
        string toString() const override;
    private:
        const int customerId;
};


class PrintVolunteerStatus : public BaseAction {
    public:
        PrintVolunteerStatus(int id);
        PrintVolunteerStatus(const PrintVolunteerStatus &other); // New Copy Constructor
        void act(WareHouse &wareHouse) override;
        PrintVolunteerStatus *clone() const override;
        string toString() const override;
    private:
        const int volunteerId;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        PrintActionsLog(const PrintActionsLog &other); // New Copy Constructor
        void act(WareHouse &wareHouse) override;
        PrintActionsLog *clone() const override;
        string toString() const override;
    private:
};

class Close : public BaseAction {
    public:
        Close();
        Close(const Close &other); // New Copy Constructor
        void act(WareHouse &wareHouse) override;
        Close *clone() const override;
        string toString() const override;
    private:
};

class BackupWareHouse : public BaseAction {
    public:
        BackupWareHouse();
        BackupWareHouse(const BackupWareHouse &other); // New Copy Constructor
        void act(WareHouse &wareHouse) override;
        BackupWareHouse *clone() const override;
        string toString() const override;
    private:
};


class RestoreWareHouse : public BaseAction {
    public:
        RestoreWareHouse();
        RestoreWareHouse(const RestoreWareHouse &other); // New Copy Constructor
        void act(WareHouse &wareHouse) override;
        RestoreWareHouse *clone() const override;
        string toString() const override;
    private:
};