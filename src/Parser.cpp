#include "../include/Parser.h"
#include "../include/WareHouse.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include <fstream>
#include <sstream>

void Parser::parseConfigurationFile(const std::string& configFilePath, WareHouse& wareHouse) {
    std::ifstream file(configFilePath);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
            std::istringstream iss(line);
            std::string key;
            iss >> key;

            if (key == "customer"){
                std::string name, customerType;
                int locationDistance, maxOrders;
                iss >> name >> customerType >> locationDistance >> maxOrders;

                Customer* customer;
                if (customerType == "soldier") {
                    customer = new SoldierCustomer(wareHouse.getCustomerCounter(), name, locationDistance, maxOrders);
                    wareHouse.addCustomer(customer);
                }
                else {
                    customer = new CivilianCustomer(wareHouse.getCustomerCounter(), name, locationDistance, maxOrders);
                    wareHouse.addCustomer(customer);
                }
            }

            else if (key == "volunteer"){
                std::string name, volunteerType;
                iss >> name >> volunteerType;
                Volunteer* volunteer;
                if (volunteerType == "collector") {
                    int cooldown;
                    iss >> cooldown;
                    volunteer = new CollectorVolunteer(wareHouse.getVolunteerCounter(), name, cooldown);
                    wareHouse.addVolunteer(volunteer);
                }
                else if (volunteerType == "limited_collector") {
                    int cooldown, maxOrders;
                    iss >> cooldown >> maxOrders;
                    volunteer = new LimitedCollectorVolunteer(wareHouse.getVolunteerCounter(), name, cooldown, maxOrders);
                    wareHouse.addVolunteer(volunteer);
                }
                else if (volunteerType == "driver"){
                    int maxDistance, distancePerStep;
                    iss >> maxDistance >> distancePerStep;
                    volunteer = new DriverVolunteer(wareHouse.getVolunteerCounter(), name, maxDistance, distancePerStep);
                    wareHouse.addVolunteer(volunteer);
                }
                else {
                    int maxDistance, distancePerStep, maxOrders;
                    iss >> maxDistance >> distancePerStep >> maxOrders;
                    volunteer = new LimitedDriverVolunteer(wareHouse.getVolunteerCounter(), name, maxDistance, distancePerStep, maxOrders);
                    wareHouse.addVolunteer(volunteer);
                }
            }
        }
    }
}