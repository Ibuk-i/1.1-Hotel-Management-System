#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

class Room
{
private:
    int roomNum;
    string roomType;
    double roomPrice;
    bool occupied;

public:
    Room(int num, string type, double price)
    {
        roomNum = num;
        roomType = type;
        roomPrice = price;
        occupied = false;
    }

    int getNumber() const { return roomNum; }
    string getType() const { return roomType; }
    double getPrice() const { return roomPrice; }
    bool getStatus() const { return occupied; }

    void bookRoom() { occupied = true; }
    void vacateRoom() { occupied = false; }

    void display() const
    {
        cout << left << setw(10) << roomNum
             << setw(15) << roomType
             << setw(10) << roomPrice
             << setw(15) << (occupied ? "Occupied" : "Available") << endl;
    }

    string toFileString() const
    {
        return to_string(roomNum) + " " + roomType + " " +
               to_string(roomPrice) + " " + (occupied ? "1" : "0");
    }
};

class Customer
{
private:
    string guestName;
    string phoneNumber;
    int bookedRoom;
    int stayDuration;

public:
    Customer(string name, string phone, int room, int duration)
    {
        guestName = name;
        cin.ignore();
        phoneNumber = phone;
        bookedRoom = room;
        stayDuration = duration;
    }

    string getName() const { return guestName; }
    string getContact() const { return phoneNumber; }
    int getRoomNumber() const { return bookedRoom; }
    int getDays() const { return stayDuration; }

    double calculateBill(const vector<Room> &rooms) const
    {
        for (const auto &r : rooms)
        {
            if (r.getNumber() == bookedRoom)
            {
                return stayDuration * r.getPrice();
            }
        }
        return 0;
    }

    void display() const
    {
        cout << "Guest Info:\n";
        cout << "Name: " << guestName << "\nContact: " << phoneNumber
             << "\nRoom: " << bookedRoom << "\nDuration: " << stayDuration << " nights\n";
    }

    string toFileString() const
    {
        return guestName + " " + phoneNumber + " " +
               to_string(bookedRoom) + " " + to_string(stayDuration);
    }
};

class Hotel
{
private:
    vector<Room> roomsList;
    vector<Customer> guestList;

    void loadRooms()
    {
        ifstream inFile("rooms.txt");
        if (!inFile)
        {
            createDefaultRooms();
            return;
        }

        int num;
        string type;
        double price;
        int isOccupied;

        while (inFile >> num >> type >> price >> isOccupied)
        {
            Room tempRoom(num, type, price);
            if (isOccupied == 1)
                tempRoom.bookRoom();
            roomsList.push_back(tempRoom);
        }
    }

    void createDefaultRooms()
    {
        for (int i = 1; i <= 10; i++)
        {
            bool isDeluxe = (i % 2 == 0);
            roomsList.push_back(
                Room(i,
                     isDeluxe ? "Deluxe" : "Standard",
                     isDeluxe ? 5000.0 : 3000.0));
        }
    }

    void saveRooms()
    {
        ofstream outFile("rooms.txt");
        for (const auto &room : roomsList)
        {
            outFile << room.toFileString() << "\n";
        }
    }

public:
    Hotel()
    {
        cout << "----------------------------------------\n";
        cout << "    Welcome to Paradise Hotel & Resort    \n";
        cout << "----------------------------------------\n";

        loadRooms();
        if (roomsList.empty())
            createDefaultRooms();
    }

    ~Hotel()
    {
        saveRooms();
    }

    void bookRoom(int roomNum, const string &name, const string &contact, int days)
    {
        for (auto &room : roomsList)
        {
            if (room.getNumber() == roomNum && !room.getStatus())
            {
                room.bookRoom();
                guestList.push_back(Customer(name, contact, roomNum, days));
                saveRooms();
                return;
            }
        }
        cout << "Room is already booked or doesn't exist.\n";
    }

    void checkOut(int roomNum)
    {
        auto it = find_if(guestList.begin(), guestList.end(),
                          [roomNum](const Customer &c)
                          { return c.getRoomNumber() == roomNum; });

        if (it != guestList.end())
        {
            cout << "Guest Details:\n";
            it->display();
            cout << "Total Bill: " << it->calculateBill(roomsList) << " Taka" << endl;
            guestList.erase(it);

            for (auto &room : roomsList)
            {
                if (room.getNumber() == roomNum)
                {
                    room.vacateRoom();
                    saveRooms();
                    return;
                }
            }
        }
        else
        {
            cout << "No booking found for this room!\n";
        }
    }

    void viewAllRooms() const
    {
        if (roomsList.empty())
        {
            cout << "No rooms available!\n";
            return;
        }
        cout << left << setw(10) << "Room" << setw(15) << "Type"
             << setw(10) << "Price" << setw(15) << "Status" << endl;
        cout << string(50, '-') << endl;
        for (const auto &room : roomsList)
        {
            room.display();
        }
    }

    void searchGuest(int roomNum) const
    {
        for (const auto &guest : guestList)
        {
            if (guest.getRoomNumber() == roomNum)
            {
                cout << "Guest Details:\n";
                guest.display();
                cout << "Total Bill: " << guest.calculateBill(roomsList) << " Taka" << endl;
                return;
            }
        }
        cout << "No guest found for this room!\n";
    }
};

int main()
{
    Hotel myHotel;
    int userChoice;

    do
    {
        cout << "\n=== Hotel Management System ===\n";
        cout << "1. Show Rooms\n2. New Booking\n3. Check Out\n"
             << "4. Find Guest\n5. Quit\n";
        cout << "Your choice: ";
        cin >> userChoice;
        cin.ignore();

        switch (userChoice)
        {
        case 1:
            myHotel.viewAllRooms();
            break;
        case 2:
        {
            string guestName, phone;
            int roomNum, nights;

            cout << "Guest name: ";
            cin >> guestName;
            cout << "Phone number: ";
            cin >> phone;
            cout << "Room number: ";
            cin >> roomNum;
            cout << "Number of nights: ";
            cin >> nights;
            cout<<endl;
            cout<<"Your room has been booked.\n";

            myHotel.bookRoom(roomNum, guestName, phone, nights);
            break;
        }
        case 3:
        {
            int roomToCheckout;
            cout << "Room number for checkout: ";
            cin >> roomToCheckout;
            myHotel.checkOut(roomToCheckout);
            break;
        }
        case 4:
        {
            int searchRoom;
            cout << "Enter room number: ";
            cin >> searchRoom;
            myHotel.searchGuest(searchRoom);
            break;
        }
        }
    } while (userChoice != 5);

    return 0;
}