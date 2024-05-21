#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        WareHouse(const WareHouse &other);
        WareHouse(WareHouse &&other); //Move Constructor
        ~WareHouse();
        const WareHouse &operator=(const WareHouse &other); 
        const WareHouse &operator=(WareHouse &&other); //Move Assignment Operator
        void clearData(); // New gerneric function that clears all the data that's allocated to the heap, for the constructors
        void start();
        int getCustomerCounter() const; // New
        int getVolunteerCounter() const; // New
        int getOrdersCounter() const; // New
        void increaseCustomerCounter(); // New increases customerCounter by 1
        void increaseVolunteerCounter(); // New increases volunteerCounter by 1
        void increaseOrdersCounter(); // New increases ordersCounter by 1
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer); // New
        void addVolunteer(Volunteer* volunteer); // New
        void deleteVolunteer(Volunteer* volunteer); // New
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void popAndPushPendingOrderCollector(Order& order, int collectorId); // New, these 4 functions meant to update orders's statuses and move them to suitable locations  
        void popAndPushPendingOrderDriver(Order& order, int driverId); // New
        void popAndPushCompletedOrderCollector(Order& order); // New
        void popAndPushCompletedOrderDriver(Order& order); // New
        void step();
        void close();
        void open();

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        vector<Volunteer*> volToDel; // New
        int customerCounter; // New For assigning unique customer IDs
        int volunteerCounter; // New For assigning unique volunteer IDs
        int ordersCounter; // New For assigning unique order IDs
        CivilianCustomer* defaultCos; // New return value in case input ID doesn't exist
        CollectorVolunteer* defaultVol; // New return value in case input ID doesn't exist
        Order* defaultOrd; // New return value in case input ID doesn't exist
};