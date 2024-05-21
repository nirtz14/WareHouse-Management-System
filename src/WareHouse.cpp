#include "../include/WareHouse.h"
#include "../include/Parser.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Action.h"
#include <iostream>
#include <sstream>
using namespace std;

WareHouse::WareHouse (const string &configFilePath) 
    : isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), volToDel(), customerCounter(0), volunteerCounter(0), ordersCounter(0), 
    defaultCos(new CivilianCustomer(-1, "Default", 0, 0)), defaultVol(new CollectorVolunteer(-1,"Default", 0)), defaultOrd(new Order(-1, -1, 0)) {
    Parser::parseConfigurationFile(configFilePath, *this);
}

WareHouse::WareHouse(const WareHouse &other) 
    : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), volToDel(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),
    ordersCounter(other.ordersCounter), defaultCos(other.defaultCos->clone()), defaultVol(other.defaultVol->clone()), defaultOrd(other.defaultOrd->clone()) {
    for (auto& customer : other.customers){
        if (customer != nullptr) {
            customers.push_back(customer->clone());
        }
    }
    for (auto& volunteer : other.volunteers){
        if (volunteer != nullptr) {
            volunteers.push_back(volunteer->clone());
        }
    }
    for (auto& order : other.pendingOrders){
        if (order != nullptr) {
            pendingOrders.push_back(order->clone());
        }
    }
    for (auto& order : other.inProcessOrders){
        if (order != nullptr) {
            inProcessOrders.push_back(order->clone());
        }
    }
    for (auto& order : other.completedOrders){
        if (order != nullptr) {
            completedOrders.push_back(order->clone());
        }
    }
    for (auto& action : other.actionsLog){
        if (action != nullptr) {
            actionsLog.push_back(action->clone());
        }
    }
    for (auto& volunteer : other.volToDel){
        if (volunteer != nullptr) {
            volToDel.push_back(volunteer->clone());
        }
    }
}

void WareHouse::clearData(){
    for (auto* customer : customers){
        delete customer;
    }
    customers.clear();
    for (auto* volunteer : volunteers){
        delete volunteer;
    }
    volunteers.clear();
    for (auto* order : pendingOrders){
        delete order;
    }
    pendingOrders.clear();
    for (auto* order : inProcessOrders){
        delete order;
    }
    inProcessOrders.clear();
    for (auto* order : completedOrders){
        delete order;
    }
    completedOrders.clear();
    for (auto* action : actionsLog){
        delete action;
    }
    actionsLog.clear();
    for (auto* volunteer : volToDel){
        delete volunteer;
    }
    volToDel.clear();
    delete defaultCos;
    delete defaultVol;
    delete defaultOrd;
}

const WareHouse &WareHouse::operator=(const WareHouse &other) {
    if (this != &other) {
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        ordersCounter = other.ordersCounter;
        clearData();
        for (auto* customer : other.customers){
            customers.push_back(customer->clone()); 
        }
        for (auto* volunteer : other.volunteers){
            volunteers.push_back(volunteer->clone());
        }
        for (auto* order : other.pendingOrders){
            pendingOrders.push_back(order->clone());
        }
        for (auto* order : other.inProcessOrders){
            inProcessOrders.push_back(order->clone());
        }
        for (auto* order : other.completedOrders){
            completedOrders.push_back(order->clone());
        }
        for (auto* action : other.actionsLog){
            actionsLog.push_back(action->clone());
        }
        for (auto* volunteer : other.volToDel){
            volToDel.push_back(volunteer->clone());
        }
        defaultCos = new CivilianCustomer(-1, "Default", 0, 0);
        defaultVol = new CollectorVolunteer(-1,"Default", 0);
        defaultOrd = new Order(-1, -1, 0);
    }
    return *this;
}

WareHouse::~WareHouse() {
    clearData();
}

WareHouse::WareHouse(WareHouse &&other)
    : isOpen(other.isOpen),
    actionsLog(move(other.actionsLog)),
    volunteers(move(other.volunteers)),
    pendingOrders(move(other.pendingOrders)),
    inProcessOrders(move(other.inProcessOrders)),
    completedOrders(move(other.completedOrders)),
    customers(move(other.customers)),
    volToDel(move(other.volToDel)),
    customerCounter(other.customerCounter), 
    volunteerCounter(other.volunteerCounter), 
    ordersCounter(other.ordersCounter),
    defaultCos(move(other.defaultCos)),
    defaultVol(move(other.defaultVol)),
    defaultOrd(move(other.defaultOrd)){}


const WareHouse &WareHouse::operator=(WareHouse &&other) {
    isOpen = other.isOpen;
    customerCounter = other.customerCounter;
    volunteerCounter = other.volunteerCounter;
    ordersCounter = other.ordersCounter;
    clearData();
    customers = move(other.customers);
    volunteers = move(other.volunteers);
    pendingOrders = move(other.pendingOrders);
    inProcessOrders = move(other.inProcessOrders);
    completedOrders = move(other.completedOrders);
    volToDel = move(other.volToDel);
    actionsLog = move(other.actionsLog);
    defaultCos = new CivilianCustomer(-1, "Default", 0, 0);
    defaultVol = new CollectorVolunteer(-1,"Default", 0);
    defaultOrd = new Order(-1, -1, 0);
    return *this;
}


void WareHouse::start() {
    open();
    cout << "Warehouse is open!" << endl;
    string input; 
    while (isOpen) {
        cout << "Enter command: ";
        getline(cin, input); 
        if (!input.empty()) {
            std::istringstream iss(input); 
            string action;
            iss >> action;
            
            if (action == "order") {
                int customerId;
                iss >> customerId;
                AddOrder* addOrder = new AddOrder(customerId);
                addOrder->act(*this);
            }
            else if (action == "orderStatus") {
                int orderId;
                iss >> orderId;
                PrintOrderStatus* printOrderStatus = new PrintOrderStatus(orderId);
                printOrderStatus->act(*this);
            }
            else if (action == "customerStatus") {
                int customerId;
                iss >> customerId;
                PrintCustomerStatus* printCustomerStatus = new PrintCustomerStatus(customerId);
                printCustomerStatus->act(*this);
            }
            else if (action == "volunteerStatus") {
                int volunteerId;
                iss >> volunteerId;
                PrintVolunteerStatus* printVolunteerStatus = new PrintVolunteerStatus(volunteerId);
                printVolunteerStatus->act(*this);
            }
            else if (action == "addCustomer" || action == "customer") {
                string name, customerType;
                int locationDistance, maxOrders;
                iss >> name >> customerType >> locationDistance >> maxOrders;
                AddCustomer* addCustomer = new AddCustomer(name, customerType, locationDistance, maxOrders);
                addCustomer->act(*this);
            }
            else if (action == "step") {
                int numOfSteps;
                iss >> numOfSteps;
                SimulateStep* simulateStep = new SimulateStep(numOfSteps);
                simulateStep->act(*this);
            }
            else if (action == "close") {
                Close* close = new Close();
                close->act(*this);
            }
            else if (action == "log") {
                PrintActionsLog* printActionsLog = new PrintActionsLog();
                printActionsLog->act(*this);
            }
           else if (action == "restore") {
               RestoreWareHouse* restoreWareHouse = new RestoreWareHouse();
               restoreWareHouse->act(*this);
           }
           else if (action == "backup") {
               BackupWareHouse* backupWareHouse = new BackupWareHouse();
               backupWareHouse->act(*this);
           }
           else {
                cout << "Invalid input" << endl;
            }
        }
        else {
            cout << "Invalid input" << endl;
        }
    }
}

int WareHouse::getCustomerCounter() const{
    return customerCounter;
}

int WareHouse::getVolunteerCounter() const{
    return volunteerCounter;
}

int WareHouse::getOrdersCounter() const{
    return ordersCounter;
}

void WareHouse:: increaseCustomerCounter(){
    customerCounter = customerCounter + 1;
}

void WareHouse:: increaseVolunteerCounter(){
    volunteerCounter = volunteerCounter + 1;
}

void WareHouse:: increaseOrdersCounter(){
    ordersCounter = ordersCounter + 1;
}

void WareHouse::addOrder(Order* order) {
    if (order != nullptr) {
        pendingOrders.push_back(order);
        increaseOrdersCounter();
    }
}

void WareHouse::addAction(BaseAction* action) {
    if (action != nullptr) {
        actionsLog.push_back(action);
    }
}

void WareHouse::addCustomer(Customer* customer){
    customers.push_back(customer);
    increaseCustomerCounter();
}

void WareHouse::addVolunteer(Volunteer* volunteer){
    volunteers.push_back(volunteer);
    increaseVolunteerCounter();
}

void WareHouse::deleteVolunteer(Volunteer* volunteer){ //in order to delete an input volunteer (from volToDel)
    for (auto it = volunteers.begin(); it != volunteers.end(); ++it){
        if ((*it) != nullptr && (*it)->getId() == volunteer->getId()){
            volunteers.erase(it);
            delete volunteer;
            return;
        }
    }
}

Customer& WareHouse::getCustomer(int customerId) const {
    for (auto* customer : WareHouse::customers){
        if (customer != nullptr && (*customer).getId() == customerId) {
            return *customer;
        }
    }
    return *defaultCos;
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    for (auto* volunteer : WareHouse::volunteers){
        if (volunteer != nullptr && (*volunteer).getId() == volunteerId) {
            return *volunteer;
        }
    }
    return *defaultVol;
}

Order& WareHouse::getOrder(int orderId) const {
    for (auto* order : WareHouse::pendingOrders){
        if (order != nullptr && (*order).getId() == orderId) {
            return *order;
        }
    }
    for (auto* order : WareHouse::inProcessOrders){
        if (order != nullptr && (*order).getId() == orderId) {
            return *order;
        }
    }
    for (auto* order : WareHouse::completedOrders){
        if (order != nullptr && (*order).getId() == orderId) {
            return *order;
        }
    }
    return *defaultOrd;
}

void WareHouse::popAndPushPendingOrderCollector(Order& order, int collectorId){
    for (auto it = pendingOrders.begin(); it != pendingOrders.end(); ++it){
        if ((*it) != nullptr && (*it)->getId() == order.getId()){
            pendingOrders.erase(it);
            order.setStatus(OrderStatus::COLLECTING);
            order.setCollectorId(collectorId);
            inProcessOrders.push_back(&order);
            return;
        }
    }
}

void WareHouse::popAndPushPendingOrderDriver(Order& order, int driverId){
    for (auto it = pendingOrders.begin(); it != pendingOrders.end(); ++it){
        if ((*it) != nullptr && (*it)->getId() == order.getId()){
            pendingOrders.erase(it);
            order.setStatus(OrderStatus::DELIVERING);
            order.setDriverId(driverId);
            inProcessOrders.push_back(&order);
            return;
        }
    }
}

void WareHouse::popAndPushCompletedOrderCollector(Order& order){
    for (auto it = inProcessOrders.begin(); it != inProcessOrders.end(); ++it){
        if ((*it) != nullptr && (*it)->getId() == order.getId()){
            inProcessOrders.erase(it);
            pendingOrders.push_back(&order);
            return;
        }
    }
}

void WareHouse::popAndPushCompletedOrderDriver(Order& order){
    for (auto it = inProcessOrders.begin(); it != inProcessOrders.end(); ++it){
        if ((*it) != nullptr && (*it)->getId() == order.getId()){
            inProcessOrders.erase(it);
            order.setStatus(OrderStatus::COMPLETED);
            completedOrders.push_back(&order);
            return;
        }
    }
}

const vector<BaseAction*>& WareHouse::getActions() const {
    return actionsLog;
}

void WareHouse::step() {
    for (auto* volunteer : WareHouse::volunteers){
        if (auto* driver = dynamic_cast<DriverVolunteer*>(volunteer)) {
            if (!driver->isBusy()){ //assigning suitable orders for free drivers 
                if (pendingOrders.size() > 0){
                    for (auto* order : pendingOrders) {
                        if (order != nullptr && driver->canTakeOrder(*order)){
                            driver->acceptOrder(*order); //updating driver's status
                            popAndPushPendingOrderDriver(*order, driver->getId()); //updatind order's status 
                        }
                    }
                }
            }
            
        }
        else if (auto* collector = dynamic_cast<CollectorVolunteer*>(volunteer)) {
            if (!collector->isBusy()) { //assigning suitable orders for free collectors
                if (pendingOrders.size() > 0){
                    for (auto* order : pendingOrders) {
                        if (order != nullptr && collector->canTakeOrder(*order)){
                            collector->acceptOrder(*order); //updating collector's status
                            popAndPushPendingOrderCollector(*order, collector->getId()); //updatind order's status 
                        }
                    }    
                }
            }
            
        }
    }
    for (auto* volunteer : WareHouse::volunteers){
        if (auto* driver = dynamic_cast<DriverVolunteer*>(volunteer)) {
            if (driver->isBusy()) { //updating both driver's and order's status (if need)
                driver->step();
                if (!driver->isBusy() && driver->getCompletedOrderId() != NO_ORDER) {
                    int exOrder = driver->getCompletedOrderId();
                    Order& oldOrder = getOrder(exOrder);
                    popAndPushCompletedOrderDriver(oldOrder);
                    if(!driver->hasOrdersLeft())
                        volToDel.push_back(driver); //in order to avoid deleting objects while iterating
                }
            }
        }
        else if (auto* collector = dynamic_cast<CollectorVolunteer*>(volunteer)) {
            if (collector->isBusy()) { //updating both collector's and order's status (if need)
                collector->step();
                if (!collector->isBusy() && collector->getCompletedOrderId() != NO_ORDER) {
                    int exOrder = collector->getCompletedOrderId();
                    Order& oldOrder = getOrder(exOrder);
                    popAndPushCompletedOrderCollector(oldOrder);
                    if(!collector->hasOrdersLeft())
                        volToDel.push_back(collector); //in order to avoid deleting objects while iterating
                }
            }  
        }
    }
    for (auto* volunteer : WareHouse::volToDel) { //deleting limited volunteers who finished their life cycle on this step
        deleteVolunteer(volunteer);
    }
    volToDel.clear();
}


void WareHouse::close() {
    isOpen = false;
    for (auto* order : pendingOrders){
        cout << "OrderID: " << order->getId() << ", CustomerID: " << order->getCustomerId() << ", Status: " << order->orderStatusToString(order->getStatus()) << endl;
    }
    for (auto* order : inProcessOrders){
        cout << "OrderID: " << order->getId() << ", CustomerID: " << order->getCustomerId() << ", Status: " << order->orderStatusToString(order->getStatus()) << endl;
    }
    for (auto* order : completedOrders){
        cout << "OrderID: " << order->getId() << ", CustomerID: " << order->getCustomerId() << ", Status: " << order->orderStatusToString(order->getStatus()) << endl;
    }
}

void WareHouse::open() {
    isOpen = true;
}