#include "../include/Volunteer.h"
#include <iostream>

Volunteer::Volunteer(int id, const std::string &name) : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

Volunteer::Volunteer(const Volunteer &other) : completedOrderId(other.completedOrderId), activeOrderId(other.activeOrderId), id(other.id), name(other.name) {}

int Volunteer::getId() const{
    return id;
}    

const std::string& Volunteer::getName() const{
    return name;
}

int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

bool Volunteer::isBusy() const{
    if(activeOrderId == NO_ORDER){
        return false;
    }
    return true;
}

//--------------------Collector----------------------------------------------

CollectorVolunteer::CollectorVolunteer(int id, const std::string &name, int coolDown):
   Volunteer(id, name), coolDown(coolDown), timeLeft(0){}

CollectorVolunteer::CollectorVolunteer(const CollectorVolunteer &other) : Volunteer(other), coolDown(other.coolDown), timeLeft(other.timeLeft) {}

CollectorVolunteer* CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}

int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

void CollectorVolunteer::setTimeLeft(int timeLeft){
    this->timeLeft = timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown(){ // Returns True if the order is completed, False otherwise
    if (timeLeft > 0){
        timeLeft--;
        return timeLeft == 0;
    }
    else if (timeLeft == 0 && activeOrderId != NO_ORDER){
        return true;
    }
    return false;
}

bool CollectorVolunteer::hasOrdersLeft() const{
    return true;
}    

bool CollectorVolunteer::canTakeOrder(const Order &order) const{
    if(activeOrderId == NO_ORDER && order.getStatus() == OrderStatus::PENDING)
        return true;
    return false;    
}

void CollectorVolunteer::acceptOrder(const Order &order){
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

void CollectorVolunteer::step() {
    if (decreaseCoolDown()) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string CollectorVolunteer::toString() const{
    string str = "VolunteerId: " + std::to_string(getId()) +
    "\nisBusy: " + (isBusy() ? "true" : "false") +
    "\nOrderID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") +
    "\nTimeleft: " + (isBusy() ? std::to_string(timeLeft) : "None") +
    "\nordersLeft: No Limit";
    return str;
}

//--------------------LimitedCollector-----------------------------------------

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const std::string &name, int coolDown, int maxOrders)
    : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(const LimitedCollectorVolunteer &other) 
    : CollectorVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft) {}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const{
    if(ordersLeft == 0){
        return false;
    }
    return true;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    if(hasOrdersLeft()){
        if(activeOrderId == NO_ORDER && order.getStatus() == OrderStatus::PENDING)
            return true;
    }
    return false;        
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order){
    activeOrderId = order.getId();
    setTimeLeft(getCoolDown());
    ordersLeft = ordersLeft - 1;
}

int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const{
    string str = "VolunteerId: " + std::to_string(getId()) +
    "\nisBusy: " + (isBusy() ? "true" : "false") +
    "\nOrderID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") +
    "\nTimeleft: " + (isBusy() ? std::to_string(getTimeLeft()) : "None") +
    "\nordersLeft: " + std::to_string(ordersLeft);
    return str;
}

//--------------------Driver------------------------------------------------

DriverVolunteer::DriverVolunteer(int id, const std::string &name, int maxDistance, int distancePerStep):
    Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

DriverVolunteer::DriverVolunteer(const DriverVolunteer &other) 
: Volunteer(other), maxDistance(other.maxDistance), distancePerStep(other.distancePerStep), distanceLeft(other.distanceLeft) {}

DriverVolunteer* DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}

void DriverVolunteer::setDistanceLeft(int distanceleft){
    this->distanceLeft = distanceLeft;
}

int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft(){ // Returns True if the order is completed, False otherwise
    if (distanceLeft > distancePerStep){
        distanceLeft -= distancePerStep;
        return false;
    }
    distanceLeft = 0;
    return true;
}

bool DriverVolunteer::hasOrdersLeft() const{
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const{
    if(activeOrderId == NO_ORDER && order.getStatus() == OrderStatus::COLLECTING && order.getDistance() <= maxDistance)
        return true;
    return false;
}

void DriverVolunteer::acceptOrder(const Order &order){
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

void DriverVolunteer::step() {
    if (decreaseDistanceLeft()) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string DriverVolunteer::toString() const{
    string str = "VolunteerId: " + std::to_string(getId()) +
    "\nisBusy: " + (isBusy() ? "true" : "false") +
    "\nOrderID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") +
    "\nDistanceLeft: " + (isBusy() ? std::to_string(distanceLeft) : "None") +
    "\nordersLeft: No Limit";
    return str;
}

//--------------------LimitedDriver------------------------------------------

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const std::string &name, int maxDistance, int distancePerStep, int maxOrders)
    : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}            

LimitedDriverVolunteer::LimitedDriverVolunteer(const LimitedDriverVolunteer &other) 
    : DriverVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft) {}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    if(ordersLeft == 0){
        return false;
    }
    return true;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const{
    if(hasOrdersLeft()){
        if(activeOrderId == NO_ORDER && order.getStatus() == OrderStatus::COLLECTING && order.getDistance() <= getMaxDistance())
            return true;
    }
    return false;
}

void LimitedDriverVolunteer::acceptOrder(const Order &order){
    activeOrderId = order.getId();
    setDistanceLeft(order.getDistance());
    ordersLeft = ordersLeft - 1;
}

string LimitedDriverVolunteer::toString() const{
    string str = "VolunteerId: " + std::to_string(getId()) +
    "\nisBusy: " + (isBusy() ? "true" : "false") +
    "\nOrderID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") +
    "\nDistanceLeft: " + (isBusy() ? std::to_string(getDistanceLeft()) : "None") +
    "\nordersLeft: " + std::to_string(ordersLeft);
    return str;
}


