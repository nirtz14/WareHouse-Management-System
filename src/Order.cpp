#include "../include/Order.h"

using namespace std;

Order::Order(int id, int customerId, int distance)
    : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

Order::Order(const Order &other) 
: id(other.id), customerId(other.customerId), distance(other.distance), status(other.status), collectorId(other.collectorId), driverId(other.driverId) {}
    
int Order::getId() const{
    return id;
}

int Order::getCustomerId() const {
    return customerId;
}

int Order::getDistance() const{
    return distance;
}

void Order::setStatus(OrderStatus status){
    (*this).status = status;
} 

void Order::setCollectorId(int collectorId) {
    (*this).collectorId = collectorId;
}

void Order::setDriverId(int driverId){
    (*this).driverId = driverId;
}

int Order::getCollectorId() const{
    return collectorId;
}

int Order::getDriverId() const{
    return driverId;
}

OrderStatus Order::getStatus() const{
    return status;
}

const string Order::toString() const{
    string str = "OrderId: " + to_string(id) +
    "\nOrderStatus: " + orderStatusToString(status) +
    "\nCustomerID: " + to_string(customerId) +
    "\nCollector: " + (collectorId == NO_VOLUNTEER ? "None" : to_string(collectorId)) +
    "\nDriver: " + (driverId == NO_VOLUNTEER ? "None" : to_string(driverId));
    return str;
}

string Order::orderStatusToString(OrderStatus status) const{
    switch (status){
        case OrderStatus::PENDING: return "PENDING";
        case OrderStatus::COLLECTING: return "COLLECTING";
        case OrderStatus::DELIVERING: return "DELIVERING";
        case OrderStatus::COMPLETED: return "COMPLETED";
    }
    return "";
}

Order* Order::clone() const{
    return new Order(*this);
}
