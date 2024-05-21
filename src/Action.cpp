#include "../include/Action.h"
#include <iostream>
using namespace std;

BaseAction::BaseAction() : errorMsg(), status() {}

BaseAction::BaseAction(const BaseAction &other) : errorMsg(other.errorMsg), status(other.status) {} // New Copy Constructor

ActionStatus BaseAction::getStatus() const {
    return status;
}

string BaseAction::ActionStatusToString(ActionStatus status) const{ // Converting ActionStatus to string
    switch (status){
        case ActionStatus::COMPLETED: return "COMPLETED";
        case ActionStatus::ERROR: return "ERROR";
    }
    return ""; 
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    (*this).errorMsg = errorMsg;
    cout << "Error: " << errorMsg << endl;
}

string BaseAction::getErrorMsg() const {
    return errorMsg;
}
//--------------------SimulateStep--------------------------------

SimulateStep::SimulateStep(int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) {}

SimulateStep::SimulateStep(const SimulateStep &other) : BaseAction(other), numOfSteps(other.numOfSteps) {} // New Copy Constructor

void SimulateStep::act(WareHouse &wareHouse) {
    wareHouse.addAction(this);
    for (int i = 0; i < numOfSteps; i++) {
        wareHouse.step();
    }
    complete();
}

string SimulateStep::toString() const{
    return "simulateStep "+to_string(numOfSteps); 
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}

//--------------------AddOrder------------------------------------

AddOrder::AddOrder(int id) : BaseAction(), customerId(id) {}

AddOrder::AddOrder(const AddOrder &other) : BaseAction(other), customerId(other.customerId) {} // New Copy Constructor

void AddOrder::act(WareHouse &wareHouse) {
    wareHouse.addAction(this);
    if (customerId < 0 || customerId >= wareHouse.getCustomerCounter()) {
        error("Customer does not exist");
    }
    else if (wareHouse.getCustomer(customerId).canMakeOrder() == false) {
        error("Cannot place this order");
    }
    else {
        Order* order = new Order(wareHouse.getOrdersCounter(), customerId, wareHouse.getCustomer(customerId).getCustomerDistance());
        wareHouse.getCustomer(customerId).addOrder(order->getId());
        wareHouse.addOrder(order);
        complete();
    }
}

string AddOrder::toString() const{
    return "order "+to_string(customerId); 
}

AddOrder* AddOrder::clone() const {
    return new AddOrder(*this);
}

//--------------------AddCustomer---------------------------------

AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders)
    : BaseAction(), customerName(customerName), customerType((customerType == "soldier") ? CustomerType::Soldier : CustomerType::Civilian), distance(distance), maxOrders(maxOrders) {}
    // When initializing of "customerType" we first check what string we got and with the ? operator we give the customer its type.

AddCustomer::AddCustomer(const AddCustomer &other) 
    : BaseAction(other), customerName(other.customerName), customerType(other.customerType), distance(other.distance), maxOrders(other.maxOrders) {} // New Copy Constructor

void AddCustomer::act(WareHouse &wareHouse) {
    wareHouse.addAction(this);
    Customer* customer;
    if (customerType == CustomerType::Soldier) {
        customer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    }
    else {
        customer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    }
    wareHouse.addCustomer(customer);
    complete();
}

string AddCustomer::CustomerTypeToString(CustomerType customerType) const{
    switch (customerType){
        case CustomerType::Soldier: return "soldier";
        case CustomerType::Civilian: return "civilian";
    }
    return "Hi:)";
}

string AddCustomer::toString() const{
    return "customer "+ customerName +" "+  CustomerTypeToString(customerType) +" "+ to_string(distance) +" "+ to_string(maxOrders);
}

AddCustomer* AddCustomer::clone() const {
    return new AddCustomer(*this);
}

//--------------------PrintOrderStatus----------------------------

PrintOrderStatus::PrintOrderStatus(int orderId) : BaseAction(), orderId(orderId) {}

PrintOrderStatus::PrintOrderStatus(const PrintOrderStatus &other) : BaseAction(other), orderId(other.orderId) {} // New Copy Constructor

void PrintOrderStatus::act(WareHouse &wareHouse) {
    wareHouse.addAction(this);
    if (orderId < 0 || orderId >= wareHouse.getOrdersCounter()) {
        error("Order doesn't exist");
    }
    else {
        cout << wareHouse.getOrder(orderId).toString() << endl;
        complete();
    }
}

PrintOrderStatus* PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const {
    return "orderStatus " + to_string(orderId);
}

//--------------------PrintCustomerStatus-------------------------

PrintCustomerStatus::PrintCustomerStatus(int customerId) : BaseAction(), customerId(customerId) {}

PrintCustomerStatus::PrintCustomerStatus(const PrintCustomerStatus &other) : BaseAction(other), customerId(other.customerId) {} // New Copy Constructor

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    wareHouse.addAction(this);
    if (customerId < 0 || customerId >= wareHouse.getCustomerCounter()) {
        error("Customer doesn't exist");
    }
    else {
        cout << "CustomerId: " << customerId << endl;
        for (auto& orderIds : wareHouse.getCustomer(customerId).getOrdersIds()) {
            cout << "OrderId: " << orderIds << endl;
            cout << "OrderStatus: " << wareHouse.getOrder(orderIds).orderStatusToString(wareHouse.getOrder(orderIds).getStatus()) << endl;
        }
        cout << "numOrdersLeft: " << (wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getNumOrders()) << endl; // "numOrdersLeft" is a function in Customer
        complete();
    }
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const {
    return "customerStatus " + to_string(customerId);
}

//--------------------PrintVolunteerStatus------------------------

PrintVolunteerStatus::PrintVolunteerStatus(int id) : BaseAction(), volunteerId(id) {}

PrintVolunteerStatus::PrintVolunteerStatus(const PrintVolunteerStatus &other) : BaseAction(other), volunteerId(other.volunteerId) {} // New Copy Constructor

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    wareHouse.addAction(this);
    if (volunteerId < 0 || volunteerId >= wareHouse.getVolunteerCounter()) {
        error("Volunteer doesn't exist");
    }
    else {
        Volunteer& volunteer = wareHouse.getVolunteer(volunteerId);
        if(volunteer.getId() == -1){
            error("Volunteer doesn't exist");
        }
        else {
            cout << "VolunteerId: " << volunteerId << endl;
            cout << "isBusy: " << (volunteer.isBusy() ? "True" : "False") << endl;
            cout << "OrderID: " << (volunteer.isBusy() ? to_string(volunteer.getActiveOrderId()) : "None") << endl;
            if (auto* driver = dynamic_cast<DriverVolunteer*>(&volunteer)) {
                cout << "TimeLeft: " << (driver->getDistanceLeft() == 0 ? "None" : to_string(driver->getDistanceLeft())) << endl;
            }
            else if (auto* collector = dynamic_cast<CollectorVolunteer*>(&volunteer)) {
                cout << "TimeLeft: " << (collector->getTimeLeft() == 0 ? "None" : to_string(collector->getTimeLeft())) << endl;
            }
            else {
                cout << "TimeLeft: None" << endl;
            }
            if (auto* limitedDriver = dynamic_cast<LimitedDriverVolunteer*>(&volunteer)) {
                cout << "OrdersLeft: " << limitedDriver->getNumOrdersLeft() << endl;
            }
            else if (auto* limitedCollector = dynamic_cast<LimitedCollectorVolunteer*>(&volunteer)) {
                cout << "OrdersLeft: " << limitedCollector->getNumOrdersLeft() << endl;
            }
            else {
                cout << "OrdersLeft: No Limit" << endl;
            }
            complete();
        }
    }
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const {
    return "volunteerStatus " + to_string(volunteerId);
}

//--------------------PrintActionsLog-----------------------------

PrintActionsLog::PrintActionsLog() : BaseAction() {}

PrintActionsLog::PrintActionsLog(const PrintActionsLog &other) : BaseAction(other) {} // New Copy Constructor

void PrintActionsLog::act(WareHouse &wareHouse) {
    for (auto& action : wareHouse.getActions()) {
        cout << (*action).toString() << " " << ActionStatusToString((*action).getStatus()) << endl;
    }
    complete();
    wareHouse.addAction(this);
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
    return "log";
}

//--------------------Close---------------------------------------

Close::Close() : BaseAction() {}

Close::Close(const Close &other) : BaseAction() {} // New Copy Constructor

void Close::act(WareHouse &wareHouse) {
    wareHouse.addAction(this);
    wareHouse.close();
    complete();
}

Close* Close::clone() const {
    return new Close(*this);
}

string Close::toString() const {
    return "Close";
}

//--------------------BackupWareHouse-----------------------------

BackupWareHouse::BackupWareHouse() : BaseAction() {}

BackupWareHouse::BackupWareHouse(const BackupWareHouse &other) : BaseAction(other) {} // New Copy Constructor

void BackupWareHouse::act(WareHouse &wareHouse) {
    wareHouse.addAction(this);
    if (backup!=nullptr)
        delete backup;
    backup = new WareHouse(wareHouse); //copying wareHouse's data into a global backup
    complete();
}

BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
    return "backup";
}

//--------------------RestoreWareHouse----------------------------

RestoreWareHouse::RestoreWareHouse() : BaseAction() {}

RestoreWareHouse::RestoreWareHouse(const RestoreWareHouse &other) : BaseAction(other) {} // New Copy Constructor

void RestoreWareHouse::act(WareHouse &wareHouse) { 
    if (backup==nullptr)
        error("No backup available");
    else {
        wareHouse = *backup; //updating given wareHouse's data from our global backup
        complete();
    }
    wareHouse.addAction(this);
}

RestoreWareHouse* RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {
    return "restore";
}