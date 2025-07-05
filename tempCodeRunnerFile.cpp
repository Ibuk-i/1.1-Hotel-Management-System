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
