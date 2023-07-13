#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>
#include <utility>
#include <stack>
#include <math.h>
#include <queue>
#include <cstring>

using namespace std;

//------------------- classes declarations-------------------------------------------------/

class ticket
{
private:
    int price;

public:
    string source;
    string destination;
    string date;
    string spacetravel_id; // to know which space_travel this ticket has been assigned to
    string passenger_id;   // to know which passenger this ticket has been assigned to
    string fbd;            // first booking date of the ticket
    void update_source(string s) { this->source = s; };
    string view_source() { return this->source; };
    void update_destination(string s) { this->destination = s; };
    string view_destination() { return this->destination; };
    void update_date(string s) { this->date = s; };
    string view_date() { return this->date; };
    void update_passenger_id(string s) { this->passenger_id = s; };
    string views_passenger_id() { return this->passenger_id; };
    void update_spacetravel_id(string s) { this->spacetravel_id = s; };
    string views_spacetravel_id() { return this->spacetravel_id; };

    void bookticket();
    void cancelticket();
    void updateticket(string source, string destination, string date);
    void updateprice();
    int getprice(); // calculate the price of the ticket based on the distance and date of travel
};

class traveller
{
private:
    string travellerid;

public:
    string name;
    string id;
    vector<ticket> list_of_tickets; // stores the list of tickets the traveller has booked

    void update_id(string id) { this->id = id; };
    void update_name(string name) { this->name = name; };
    string getname() { return this->name; };
    string getid() { return this->id; };

    void getticket();         //  lists all the tickets the user has booked
    bool verifyid(string id); // verify if the user exists in the the corresponding list
};

// subclasss astronaut
class astronaut : public traveller
{
private:
    int experience;

public:
    void update_exp(int n) { this->experience = n; }
    int show_exp() { return this->experience; }
};
// subclass passenger
class passenger : public traveller
{
private:
    int validity;

public:
    void update_val(int n) { this->validity = n; }
    int show_val() { return this->validity; }
};
// subclass commander
class commander : public traveller
{
private:
    string authority;

public:
    int experience;
    string licenseid;
    void update_auth(string s) { this->authority = s; }
    string show_auth() { return this->authority; }
};
// planet class
class planet
{
public:
    float x;
    float y;
    float z;
    void setx(float n) { this->x = n; };
    void sety(float n) { this->y = n; };
    void setz(float n) { this->z = n; };
    float getx();
    float gety();
    float getz();
};
// space travel class
class space_travel
{
private:
    commander com;
    astronaut astro;

public:
    string space_travel_id;             // stores the unique id of the space travel
    string source;                      // source of travel
    string destination;                 // destination of travel
    string date;                        // date of travel
    vector<string> list_of_passenger;   // stores the list of all the passengers ids that are added to this travel

    void add_pass(string passenger_id); // adds passenger to passengers list
    void list_all_trav();               // lists all the travellers including passengers , commanders and astronauts
    void set_astro_id(string astro);
    void set_com_id(string com);
    void update_astro_id(string astro);
    void update_com_id(string com);
    string get_astro_id() { return astro.id; }
    string get_com_id() { return com.id; }
};

/*---------------------------------------------xxxxxx----------------------------------------------------------xxxxxx-----------------------------------------------------------*/
map<string, passenger> universal_passenger_list;        // stores all the passenger object in the universe
map<string, astronaut> universal_astronaut_list;        // stores all the astronaut object in the universe
map<string, commander> universal_commander_list;        // stores all the commander object in the universe
map<string, planet> universal_planet_list;              // stores all the planet object in the universe

map<string, space_travel> universal_spacetravel_list;   // stores all the space_travel object in the universe
map<string, int> sd_st_ct;                              // same day same trip space travel count;
map<string, ticket> unassigned_ticket_list;             // stores all the tickets that havent been assigned a space travel
map<string, int> sdt_unassigned_ct;                     // same day same trip   unassigned ticket ticket count

string G_TIME; // os time
int gtime;     // stores os time in a specific desired format

// list all the ticket of the traveller
void traveller ::getticket()
{
    if (this->list_of_tickets.size() == 0)
    {
        cout << "Empty \n";
        return;
    }
    for (int i = 0; i < this->list_of_tickets.size(); ++i)
    {
        cout << "ticket id = " << (this->list_of_tickets)[i].spacetravel_id << " , source = " << (this->list_of_tickets)[i].source << ", destination = " << (this->list_of_tickets)[i].destination << " , date = " << (this->list_of_tickets)[i].date << "\n";
    }
}
// verify traveller id
bool traveller ::verifyid(string id)
{
    if (id == this->id)
        return true;
    else
        return false;
}

// cancel the ticket from the sapce travel class and passenger ticket list
void ticket ::cancelticket()
{
    string spacetravel = this->spacetravel_id;
    string passenger_id = this->passenger_id;
    vector<string> new_list_of_passenger;

    if (universal_spacetravel_list.find(spacetravel) == universal_spacetravel_list.end())
    {
        return;
    }

    for (int i = 0; i < (universal_spacetravel_list[spacetravel].list_of_passenger).size(); ++i)
    {
        if ((universal_spacetravel_list[spacetravel].list_of_passenger)[i] == passenger_id)
            ;
        else
        {
            new_list_of_passenger.push_back((universal_spacetravel_list[spacetravel].list_of_passenger)[i]);
        }
    }
    universal_spacetravel_list[spacetravel].list_of_passenger = new_list_of_passenger;
    // if list of passengers in space travel contains only one ticket after update then we need to delete that travel and put the only ticket in unassigned section
    if (new_list_of_passenger.size() == 1)
    {
        string lone_passenger_id = new_list_of_passenger[0];
        universal_spacetravel_list.erase(spacetravel);
        string temp = this->source + this->destination = this->date;
        sd_st_ct[temp]--;
        // now we need to add this to unassigned ticket data
        sdt_unassigned_ct[temp]++;
        ticket temp_ticket;
        temp_ticket = (*this);
        unassigned_ticket_list[temp] = temp_ticket;
    }
}
// update the ticket
void ticket ::updateticket(string source, string destination, string date)
{
}

// calculate the price of thee ticket
void ticket ::updateprice()
{
    float x, y, z;
    float a, b, c;
    x = universal_planet_list[this->source].getx();
    y = universal_planet_list[this->source].gety();
    z = universal_planet_list[this->source].getz();
    a = universal_planet_list[this->destination].getx();
    b = universal_planet_list[this->destination].gety();
    c = universal_planet_list[this->destination].getz();
    float temp = (x - a) * (x - a) + (y - b) * (y - b) + (z - c) * (z - c);
    string temp_date = this->date;
    int d = (temp_date[0] - '0') * 10 + (temp_date[1] - '0');
    int m = (temp_date[2] - '0') * 10 + (temp_date[3] - '0');
    int yy = (temp_date[4] - '0') * 1000 + (temp_date[3] - '5') * 100 + (temp_date[2] - '6') * 10 + (temp_date[7] - '0');
    int time = d + m * 30 + yy * 365;
    this->price = (sqrt(temp) * 1000 * gtime) / time;
}

int ticket ::getprice()
{
    return this->price;
}

float planet ::getx()
{
    return x;
}

float planet ::gety()
{
    return y;
}

float planet ::getz()
{
    return z;
}

void space_travel ::add_pass(string passenger_id)
{
    this->list_of_passenger.push_back(passenger_id);
    cout << "passenger : " << passenger_id << " successfully added as passenger for space travel " << this->space_travel_id << "\n";
}

void space_travel ::list_all_trav() {}

void space_travel ::set_astro_id(string astro)
{
    ticket temp;
    temp.date = this->date;
    temp.fbd = this->date;
    temp.source = this->source;
    temp.destination = this->destination;
    temp.passenger_id = astro;
    temp.updateprice();
    (universal_astronaut_list[astro].list_of_tickets).push_back(temp);
    this->astro = universal_astronaut_list[astro];
    cout << "Astronaut : " << astro << " successfully added as astronaut for space travel " << this->space_travel_id << "\n";
}

void space_travel ::set_com_id(string com)
{
    ticket temp;
    temp.date = this->date;
    temp.fbd = this->date;
    temp.source = this->source;
    temp.destination = this->destination;
    temp.passenger_id = com;
    temp.updateprice();
    (universal_commander_list[com].list_of_tickets).push_back(temp);
    this->com = universal_commander_list[com];
    cout << "commander : " << com << " successfully added as commander for space travel " << this->space_travel_id << "\n";
}

void space_travel ::update_astro_id(string astro)
{

    ticket temp;
    temp.date = this->date;
    temp.fbd = this->date;
    temp.source = this->source;
    temp.destination = this->destination;
    temp.passenger_id = astro;
    temp.updateprice();
    (universal_astronaut_list[astro].list_of_tickets).push_back(temp);
    this->astro = universal_astronaut_list[astro];
    cout << "Astronaut : " << astro << " successfully updated as astronaut for space travel " << this->space_travel_id << "\n";
}

void space_travel ::update_com_id(string com)
{
    ticket temp;
    temp.date = this->date;
    temp.fbd = this->date;
    temp.source = this->source;
    temp.destination = this->destination;
    temp.passenger_id = com;
    temp.updateprice();
    (universal_commander_list[com].list_of_tickets).push_back(temp);
    this->com = universal_commander_list[com];
    cout << "commander : " << com << " successfully updated as commander for space travel " << this->space_travel_id << "\n";
}

void create_universe()
{
    passenger temp;
    for (int i = 1; i <= 15; ++i)
    {
        temp.id = "pass" + to_string(i);
        temp.name = "passenger" + to_string(i);
        universal_passenger_list[temp.id] = temp;
    }
    astronaut astro;
    astro.id = "id1";
    astro.name = "John";
    astro.update_exp(10);
    universal_astronaut_list[astro.id] = astro;

    astro.id = "id2";
    astro.name = "Sam";
    astro.update_exp(18);
    universal_astronaut_list[astro.id] = astro;

    commander com;
    com.id = "id3";
    com.name = "Alice";
    com.update_auth("Master");
    universal_commander_list[com.id] = com;

    com.id = "id4";
    com.name = "Anna";
    com.update_auth("master");
    universal_commander_list[com.id] = com;

    planet plan;
    string plan_name = "Mars";
    plan.x = 200;
    plan.y = 400;
    plan.z = 400.5;
    universal_planet_list[plan_name] = plan;

    plan_name = "Earth";
    plan.x = 200;
    plan.y = 300;
    plan.z = 100.5;
    universal_planet_list[plan_name] = plan;

    plan_name = "Krypton";
    plan.x = 20000;
    plan.y = 9000.9;
    plan.z = 6969.69;
    universal_planet_list[plan_name] = plan;

    plan_name = "Xandar";
    plan.x = 20090.99;
    plan.y = 90576.50;
    plan.z = 6969.69;
    universal_planet_list[plan_name] = plan;
}


// creates the required space travel for a trip if the trip already doesnt exist or is full
void create_space_travel(ticket &ticket1, string other_passenger, string other_ticket_id, string space_travelid)
{
    space_travel newtravel;
    string astro, com;

    newtravel.space_travel_id = space_travelid;
    newtravel.date = ticket1.date;
    newtravel.source = ticket1.source;
    newtravel.destination = ticket1.destination;

    // search the required ticket in other passenger ticket list;
    for (int i = 0; i < (universal_passenger_list[other_passenger].list_of_tickets).size(); i++)
    {
        if (((universal_passenger_list[other_passenger].list_of_tickets)[i]).spacetravel_id == "w-" + other_ticket_id)
        {
            ((universal_passenger_list[other_passenger].list_of_tickets)[i]).spacetravel_id = space_travelid;
        }
    }

    ticket1.spacetravel_id = space_travelid;

    newtravel.add_pass(ticket1.passenger_id);
    newtravel.add_pass(other_passenger);

    cout << "Enter astronaut id (from the list of astronauts) that is to be assigned for space travel :" << space_travelid << " : \n";
    for (auto it = universal_astronaut_list.begin(); it != universal_astronaut_list.end(); it++)
    {
        cout << "Astronaut id = " << it->first << ", Astronaut name = " << it->second.name << ", Experience : " << it->second.show_exp() << "\n";
    }
    cin >> astro;
    while (universal_astronaut_list.find(astro) == universal_astronaut_list.end())
    {
        cout << "Enter valid astronaut id from the list :";
        cin >> astro;
    }
    newtravel.set_astro_id(astro);

    cout << "Enter commander id (from the list of commanders) that is to be assigned for space travel :" << space_travelid << " : \n";
    for (auto it = universal_commander_list.begin(); it != universal_commander_list.end(); it++)
    {
        cout << "commander id = " << it->first << ", commander name = " << it->second.name << ", Experience : " << it->second.show_auth() << "\n";
    }
    cin >> com;
    while (universal_commander_list.find(com) == universal_commander_list.end())
    {
        cout << "Enter valid commander id from the list :";
        cin >> com;
    }
    newtravel.set_com_id(com);

    universal_spacetravel_list[space_travelid] = newtravel;
}

// assigns travel to tickets based on availability
void assign_travel(ticket &ticket)
{
    string source_planet = ticket.source;
    string destination_planet = ticket.destination;
    string date_of_travel = ticket.date;
    string passenger_id = ticket.passenger_id;

    string space_travel_id = source_planet + destination_planet + date_of_travel;

    string temp_space_travel_id = space_travel_id + to_string(sd_st_ct[space_travel_id]);
    auto it = universal_spacetravel_list.find(temp_space_travel_id);

    if (it == universal_spacetravel_list.end())
    {
        if (sdt_unassigned_ct[space_travel_id] > 0)
        {
            auto other_ticket = unassigned_ticket_list[space_travel_id];
            string other_passenger = other_ticket.passenger_id;
            create_space_travel(ticket, other_passenger, space_travel_id, temp_space_travel_id);
            sdt_unassigned_ct[space_travel_id]--;
            unassigned_ticket_list.erase(space_travel_id);
        }
        else
        {
            ticket.spacetravel_id = "w-" + space_travel_id;
            sdt_unassigned_ct[space_travel_id]++;
            unassigned_ticket_list[space_travel_id] = ticket;
        }
    }
    else if (it != universal_spacetravel_list.end() && ((it->second).list_of_passenger).size() > 8)
    {
        if (sdt_unassigned_ct[space_travel_id] > 0)
        {
            auto other_ticket = unassigned_ticket_list[space_travel_id];
            string other_passenger = other_ticket.passenger_id;
            sd_st_ct[space_travel_id]++;
            temp_space_travel_id = space_travel_id + to_string(sd_st_ct[space_travel_id]);
            create_space_travel(ticket, other_passenger, space_travel_id, temp_space_travel_id);
            sdt_unassigned_ct[space_travel_id]--;
            unassigned_ticket_list.erase(space_travel_id);
        }
        else
        {
            ticket.spacetravel_id = "w-" + space_travel_id;
            sdt_unassigned_ct[space_travel_id]++;
            unassigned_ticket_list[space_travel_id] = ticket;
        }
    }
    else
    {
        universal_spacetravel_list[temp_space_travel_id].add_pass(passenger_id);
        ticket.spacetravel_id = temp_space_travel_id;
    }
}
// books the return ticket for a round trip
void book_return_ticket(string passenger_id, string source_planet, string destination_planet)
{
    string date_of_travel;
    cout << "Enter date of return travel in DDMMYYYY format : ";
    cin >> date_of_travel;
    int d = (date_of_travel[0] - '0') * 10 + (date_of_travel[1] - '0');
    int m = (date_of_travel[2] - '0') * 10 + (date_of_travel[3] - '0');
    int y = (date_of_travel[4] - '0') * 1000 + (date_of_travel[5] - '0') * 100 + (date_of_travel[6] - '0') * 10 + (date_of_travel[7] - '0');
    int temp = d + m * 30 + y * 365;
    while (temp > gtime + 10 * 365)
    {
        cout << "Invalid travel date, please enter a date within 10 years of booking in DDMMYYYY format:";
        cin >> date_of_travel;
        d = (date_of_travel[0] - '0') * 10 + (date_of_travel[1] - '0');
        m = (date_of_travel[2] - '0') * 10 + (date_of_travel[3] - '0');
        y = (date_of_travel[4] - '0') * 1000 + (date_of_travel[5] - '0') * 100 + (date_of_travel[6] - '0') * 10 + (date_of_travel[7] - '0');
        temp = d + m * 30 + y * 365;
    }

    ticket temp_ticket;
    temp_ticket.source = source_planet;
    temp_ticket.destination = destination_planet;
    temp_ticket.date = date_of_travel;
    temp_ticket.fbd = date_of_travel;
    temp_ticket.passenger_id = passenger_id;
    temp_ticket.updateprice();

    assign_travel(temp_ticket);

    universal_passenger_list[passenger_id].list_of_tickets.push_back(temp_ticket);
}

// books the ticket based on user input
void book_ticket()
{
    string passenger_id;
    string source_planet;
    string destination_planet;
    string date_of_travel;

    cout << "Enter passenger id from the passenger list : \n";
    for (auto it = universal_passenger_list.begin(); it != universal_passenger_list.end(); it++)
    {
        cout << "passenger id = " << it->first << " passenger name = " << it->second.name << "\n";
    }
    cout << "Enter : ";
    cin >> passenger_id;
    while (universal_passenger_list.find(passenger_id) == universal_passenger_list.end())
    {
        cout << "Invalid passenger id , please enter valid passenger id :";
        cin >> passenger_id;
    }

    cout << "\nEnter source planet from the planet list: ";
    for (auto it = universal_planet_list.begin(); it != universal_planet_list.end(); it++)
    {
        cout << "Planet Name = " << it->first << ", x co-ordinate = " << it->second.x << ", y co-ordinate = " << it->second.y << ", z co-ordinate = " << it->second.z << "\n";
    }
    cout << "Enter : ";
    cin >> source_planet;
    while (universal_planet_list.find(source_planet) == universal_planet_list.end())
    {
        cout << "Invalid source planet , please re-enter : ";
        cin >> source_planet;
    }

    cout << "\nEnter destination planet from the planet list: ";
    for (auto it = universal_planet_list.begin(); it != universal_planet_list.end(); it++)
    {
        cout << " Planet Name = " << it->first << ", x co-ordinate = " << it->second.x << ", y co-ordinate = " << it->second.y << ", z co-ordinate = " << it->second.z << "\n";
    }
    cout << "Enter : ";
    cin >> destination_planet;
    while (universal_planet_list.find(destination_planet) == universal_planet_list.end() || source_planet == destination_planet)
    {
        cout << "Invalid detination planet , please re-enter : ";
        cin >> destination_planet;
    }

    cout << "\nEnter date of travel in DDMMYYYY format : ";
    cin >> date_of_travel;
    int d = (date_of_travel[0] - '0') * 10 + (date_of_travel[1] - '0');
    int m = (date_of_travel[2] - '0') * 10 + (date_of_travel[3] - '0');
    int y = (date_of_travel[4] - '0') * 1000 + (date_of_travel[5] - '0') * 100 + (date_of_travel[6] - '0') * 10 + (date_of_travel[7] - '0');
    int temp = d + m * 30 + y * 365;
    while (temp > gtime + 10 * 365)
    {
        cout << "Invalid travel date, please enter a date within 10 years of booking in DDMMYYYY format:";
        cin >> date_of_travel;
        d = (date_of_travel[0] - '0') * 10 + (date_of_travel[1] - '0');
        m = (date_of_travel[2] - '0') * 10 + (date_of_travel[3] - '0');
        y = (date_of_travel[4] - '0') * 1000 + (date_of_travel[5] - '0') * 100 + (date_of_travel[6] - '0') * 10 + (date_of_travel[7] - '0');
        temp = d + m * 30 + y * 365;
    }

    ticket temp_ticket;
    temp_ticket.source = source_planet;
    temp_ticket.destination = destination_planet;
    temp_ticket.date = date_of_travel;
    temp_ticket.fbd = date_of_travel;
    temp_ticket.passenger_id = passenger_id;
    temp_ticket.updateprice();

    assign_travel(temp_ticket);

    universal_passenger_list[passenger_id].list_of_tickets.push_back(temp_ticket);

    int i;
    cout << "Enter 1 to book the return ticket \n";
    cout << "Enter 0 not to \n";
    cout << "Enter : ";
    cin >> i;
    switch (i)
    {
    case 1:
    {
        book_return_ticket(passenger_id, destination_planet, source_planet);
        cout << " Return ticket booked successfully\n";
        break;
    }
    default:
    {
        ;
    }
    }
}
// cancel the tickets based on user input
void cancel_ticket()
{
    string passenger_id;
    string ticket_id;

    cout << "Enter passenger id from the passenger list : ";
    for (auto it = universal_passenger_list.begin(); it != universal_passenger_list.end(); it++)
    {
        cout << "passenger id = " << it->first << " passenger name = " << it->second.name << "\n";
    }
    cin >> passenger_id;
    while (universal_passenger_list.find(passenger_id) == universal_passenger_list.end())
    {
        cout << "Invalid passenger id , please enter valid passenger id :";
        cin >> passenger_id;
    }
    // if traveller_id is not in universal traveller list re-enter
    cout << "choose the ticket to be cancelled from the list of tickets booked by the current user : \n";
    universal_passenger_list[passenger_id].getticket();
    cout << " Enter the ticket id to be cancelled : ";
    cin >> ticket_id;

    int j = 0;
    vector<ticket> new_list_of_tickets;
    for (int i; i < (universal_passenger_list[passenger_id].list_of_tickets).size(); i++)
    {
        if (((universal_passenger_list[passenger_id].list_of_tickets)[i]).spacetravel_id == ticket_id)
            j = i;
        else
        {
            new_list_of_tickets.push_back((universal_passenger_list[passenger_id].list_of_tickets)[i]);
        }
    }
    ((universal_passenger_list[passenger_id].list_of_tickets)[j]).cancelticket();
    universal_passenger_list[passenger_id].list_of_tickets = new_list_of_tickets;
}
// update the ticket based on user input
void update_ticket()
{
    string passenger_id;
    string ticket_id;

    cout << "Enter passenger id from the passenger list whose ticket is to be updated : \n";
    for (auto it = universal_passenger_list.begin(); it != universal_passenger_list.end(); it++)
    {
        cout << "passenger id = " << it->first << " passenger name = " << it->second.name << "\n";
    }
    cout << "Enter : ";
    cin >> passenger_id;
    while (universal_passenger_list.find(passenger_id) == universal_passenger_list.end())
    {
        cout << "Invalid passenger id , please enter valid passenger id :";
        cin >> passenger_id;
    }

    cout << "Choose the ticket to be updated from the list of tickets booked by the current user : \n";
    universal_passenger_list[passenger_id].getticket();
    if ((universal_passenger_list[passenger_id].list_of_tickets).size() == 0)
    {
        cout << "No tickets to be updated\n";
        return;
    }
    cout << "Enter the ticket id to be updated : ";
    cin >> ticket_id;

    int j = 0;
    vector<ticket> new_list_of_tickets;
    for (int i = 0; i < (universal_passenger_list[passenger_id].list_of_tickets).size(); i++)
    {
        if (((universal_passenger_list[passenger_id].list_of_tickets)[i]).spacetravel_id == ticket_id)
            j = i;
        else
        {
            new_list_of_tickets.push_back((universal_passenger_list[passenger_id].list_of_tickets)[i]);
        }
    }
    string source_planet = ((universal_passenger_list[passenger_id].list_of_tickets)[j]).source;
    string fbdn = ((universal_passenger_list[passenger_id].list_of_tickets)[j]).fbd;
    string destination_planet;
    string date_of_travel;

    int dd = (fbdn[0] - '0') * 10 + (fbdn[1] - '0');
    int mm = (fbdn[2] - '0') * 10 + (fbdn[3] - '0');
    int yy = (fbdn[4] - '0') * 1000 + (fbdn[5] - '0') * 100 + (fbdn[6] - '0') * 10 + (fbdn[7] - '0');
    int ftemp = dd + mm * 30 + yy * 365;

    /// choose from available list
    cout << "\nEnter updated destination planet from the planet list: \n";
    for (auto it = universal_planet_list.begin(); it != universal_planet_list.end(); it++)
    {
        cout << " Planet Name = " << it->first << ", x co-ordinate = " << it->second.x << ", y co-ordinate = " << it->second.y << ", z co-ordinate = " << it->second.z << "\n";
    }
    cout << "Enter : ";
    cin >> destination_planet;
    while (universal_planet_list.find(destination_planet) == universal_planet_list.end() || source_planet == destination_planet)
    {
        cout << "Invalid destination planet , please re-enter : \n";
        cout << "Enter : ";
        cin >> destination_planet;
    }

    cout << "\nEnter updated date of travel in DDMMYYYY format : ";
    cin >> date_of_travel;
    int d = (date_of_travel[0] - '0') * 10 + (date_of_travel[1] - '0');
    int m = (date_of_travel[2] - '0') * 10 + (date_of_travel[3] - '0');
    int y = (date_of_travel[4] - '0') * 1000 + (date_of_travel[5] - '0') * 100 + (date_of_travel[6] - '0') * 10 + (date_of_travel[7] - '0');
    int temp = d + m * 30 + y * 365;

    if (temp > ftemp + 10 * 365)
    {
        cout << "Invalid update date\n";
        cout << "Ticket update failed\n";
        cout << "\n";
        return;
    }
    string check = source_planet + destination_planet + date_of_travel;
    if (ticket_id == check || ticket_id == "w-" + check)
    {
        cout << "Invalid update : return ticket update ticket is same as before \n";
        return;
    }
    ticket temp_ticket;
    temp_ticket.source = ((universal_passenger_list[passenger_id].list_of_tickets)[j]).source;
    temp_ticket.destination = destination_planet;
    temp_ticket.date = date_of_travel;
    temp_ticket.fbd = fbdn;
    temp_ticket.passenger_id = passenger_id;
    temp_ticket.updateprice();

    assign_travel(temp_ticket);
    new_list_of_tickets.push_back(temp_ticket);
    // cancel old tick
    ((universal_passenger_list[passenger_id].list_of_tickets)[j]).cancelticket();
    universal_passenger_list[passenger_id].list_of_tickets = new_list_of_tickets;
    cout << "ticket succesfully updated \n";
}
// interactive user input function
int user_input()
{
    int i = 0;
    cout << "Enter 0 to exit \n";
    cout << "Enter 1 to login as user\n";
    cout << "Enter 2 to login as administrator\n";

    cin >> i;
    return i;
}
// displays all the objects in the universe
void show_universe()
{
    cout << "\nPassengers in the current universe are as follows: \n";
    for (auto it = universal_passenger_list.begin(); it != universal_passenger_list.end(); it++)
    {
        cout << "passenger id = " << it->first << " passenger name = " << it->second.name << "\n";
    }

    cout << "\nAstronauts in the current universe are as follows: \n";
    for (auto it = universal_astronaut_list.begin(); it != universal_astronaut_list.end(); it++)
    {
        cout << "Astronaut id = " << it->first << ", Astronaut name = " << it->second.name << ", Experience : " << it->second.show_exp() << "\n";
    }

    cout << "\nCommanders in the current universe are as follows: \n";
    for (auto it = universal_commander_list.begin(); it != universal_commander_list.end(); it++)
    {
        cout << "Commander id = " << it->first << ", Commander name = " << it->second.name << ", Authority : " << it->second.show_auth() << "\n";
    }

    cout << "\nPlanets in the current universe are as follows: \n";
    for (auto it = universal_planet_list.begin(); it != universal_planet_list.end(); it++)
    {
        cout << "Planet Name = " << it->first << ", x co-ordinate = " << it->second.x << ", y co-ordinate = " << it->second.y << ", z co-ordinate = " << it->second.z << "\n";
    }

    if (universal_spacetravel_list.size() == 0)
    {
        cout << "\nCurrently the universe has no scheduled space travels : \n";
        cout << endl;
        return;
    }
    cout << "\nSpace Travels in the current universe are as follows: \n";
    for (auto it = universal_spacetravel_list.begin(); it != universal_spacetravel_list.end(); it++)
    {
        cout << " Space travel id = " << it->first << "\n  source = " << it->second.source << " ,destination = " << it->second.destination << " ,date = " << it->second.date << "\n";
        cout << "  commander = " << it->second.get_com_id() << "\n  astronaut = " << it->second.get_astro_id() << "\n  No of passengers =" << it->second.list_of_passenger.size() << "\n";
    }
    cout << endl;
}
// allows to add traveller
void add_traveller()
{
    int i = 0;
    cout << "Enter 0 to exit add traveller option :\n";
    cout << "Enter 1 to add passenger :\n";
    cout << "Enter 2 to add astronaut :\n";
    cout << "Enter 3 to add commander :\n";
    cin >> i;
    switch (i)
    {
    case 1:
    {
        passenger temp;
        cout << "Enter passenger id : ";
        cin >> temp.id;
        cout << "Enter passenger name : ";
        cin >> temp.name;
        // check if passenger already exits
        if (universal_passenger_list.find(temp.id) != universal_passenger_list.end())
        {
            cout << "Passenger with given id already exist, Retry with differnt id\n";
            return;
        }
        else
            universal_passenger_list[temp.id] = temp;
        break;
    }
    case 2:
    {
        astronaut temp;
        int n;
        cout << "Enter astronaut id : ";
        cin >> temp.id;
        cout << "Enter astronaut name : ";
        cin >> temp.name;
        cout << "Enter years of experience : ";
        cin >> n;
        temp.update_exp(n);
        // check if astronaut already exits
        if (universal_astronaut_list.find(temp.id) != universal_astronaut_list.end())
        {
            cout << "astronaut with given id already exist :-(, Retry with differnt id :-)\n";
            return;
        }
        universal_astronaut_list[temp.id] = temp;
        break;
    }
    case 3:
    {
        commander temp;
        string s;
        cout << "Enter commander id : ";
        cin >> temp.id;
        cout << "Enter commander name : ";
        cin >> temp.name;
        cout << "Enter authority : ";
        cin >> s;
        temp.update_auth(s);
        // check if commander already exits
        if (universal_commander_list.find(temp.id) != universal_commander_list.end())
        {
            cout << "commander with given id already exist, Retry with differnt id\n";
            return;
        }
        else
            universal_commander_list[temp.id] = temp;

        break;
    }
    default:
    {
        ;
    }
    }
}
// allows to add planet
void add_planet()
{
    planet plan;
    string plan_name;
    cout << "Enter Planet Name : ";
    cin >> plan_name;
    cout << "Enter x-coordinate : ";
    cin >> plan.x;
    cout << "Enter y-coordinate : ";
    cin >> plan.y;
    cout << "Enter z-coordinate : ";
    cin >> plan.z;
    universal_planet_list[plan_name] = plan;
}
// view travellers detail
void enter_as_traveller()
{
    int i = 0;
    cout << "Enter 0 to exit view journey option :\n";
    cout << "Enter 1 to for passenger :\n";
    cout << "Enter 2 to for astronaut :\n";
    cout << "Enter 3 to for commander :\n";
    cin >> i;
    switch (i)
    {
    case 1:
    {
        string passenger_id;
        cout << "Enter passenger id from the passenger list : \n";
        for (auto it = universal_passenger_list.begin(); it != universal_passenger_list.end(); it++)
        {
            cout << "passenger id = " << it->first << " passenger name = " << it->second.name << "\n";
        }
        cin >> passenger_id;
        while (universal_passenger_list.find(passenger_id) == universal_passenger_list.end())
        {
            cout << "Invalid passenger id , please enter valid passenger id :";
            cin >> passenger_id;
        }
        cout << "All Ticket details :\n";
        for (int j = 0; j < (universal_passenger_list[passenger_id].list_of_tickets).size(); j++)
        {
            ticket temp = (universal_passenger_list[passenger_id].list_of_tickets)[j];
            cout << " ticket id :" << temp.spacetravel_id << "\n";
            cout << "  source : " << temp.source << " ,destination : " << temp.destination << " ,price : " << temp.getprice() << "\n";
            cout << "  date : " << temp.date << "\n";
        }

        break;
    }
    case 2:
    {
        string astronaut_id;
        cout << "Enter astronaut id from the astronaut list : \n";
        for (auto it = universal_astronaut_list.begin(); it != universal_astronaut_list.end(); it++)
        {
            cout << "astronaut id = " << it->first << " astronaut name = " << it->second.name << "\n";
        }
        cin >> astronaut_id;
        while (universal_astronaut_list.find(astronaut_id) == universal_astronaut_list.end())
        {
            cout << "Invalid astronaut id , please enter valid astronaut id :";
            cin >> astronaut_id;
        }
        cout << "All Ticket details :\n";
        for (int j = 0; j < (universal_astronaut_list[astronaut_id].list_of_tickets).size(); j++)
        {
            ticket temp = (universal_astronaut_list[astronaut_id].list_of_tickets)[j];
            cout << " ticket id :" << temp.spacetravel_id << "\n";
            cout << "  source : " << temp.source << " ,destination : " << temp.destination << " ,price : " << temp.getprice() << "\n";
            cout << "  date : " << temp.date << "\n";
        }

        break;
    }
    case 3:
    {
        string commander_id;
        cout << "Enter commander id from the commander list : \n";
        for (auto it = universal_commander_list.begin(); it != universal_commander_list.end(); it++)
        {
            cout << "commander id = " << it->first << " commander name = " << it->second.name << "\n";
        }
        cin >> commander_id;
        while (universal_commander_list.find(commander_id) == universal_commander_list.end())
        {
            cout << "Invalid commander id , please enter valid commander id :";
            cin >> commander_id;
        }
        cout << "All Ticket details :\n";
        for (int j = 0; j < (universal_commander_list[commander_id].list_of_tickets).size(); j++)
        {
            ticket temp = (universal_commander_list[commander_id].list_of_tickets)[j];
            cout << " ticket id :" << temp.spacetravel_id << "\n";
            cout << "  source : " << temp.source << " ,destination : " << temp.destination << " ,price : " << temp.getprice() << "\n";
            cout << "  date : " << temp.date << "\n";
        }
        break;
    }
    default:
    {
        ;
    }
    }
}
// admin ui
void admin_access()
{
    int i = 0;
    cout << "Enter 0 to exit admin view:\n";
    cout << "Enter 1 to add planet :\n";
    cout << "Enter 2 to view universe:\n";
    cout << "Enter 3 to view travellers travel detail:\n";
    cin >> i;
    while (i)
    {
        switch (i)
        {
        case 1:
        {
            add_planet();
            break;
        }
        case 2:
        {
            show_universe();
            break;
        }
        case 3:
        {
            enter_as_traveller();
            break;
        }
        default:
            break;
        }

        cout << "Enter 0 to exit admin view:\n";
        cout << "Enter 1 to add planet :\n";
        cout << "Enter 2 to view universe:\n";
        cout << "Enter 3 to view travellers travel detail:\n";
        cin >> i;
    }
}
// user ui
void user_access()
{
    int i = 0;
    cout << "Enter 0 to exit user view:\n";
    cout << "Enter 1 to register traveller :\n";
    cout << "Enter 2 to book ticket:\n";
    cout << "Enter 3 to update ticket:\n";
    cout << "Enter 4 to cancel ticket:\n";
    cout << "Enter 5 to view travellers travel detail:\n";
    cout << "Enter 6 to view universe:\n";
    cin >> i;
    while (i)
    {
        switch (i)
        {
        case 1:
        {
            add_traveller();
            ;
            break;
        }
        case 2:
        {
            book_ticket();
            break;
        }
        case 3:
        {
            update_ticket();
            break;
        }
        case 4:
        {
            cancel_ticket();
            break;
        }
        case 5:
        {
            enter_as_traveller();
            break;
        }
        case 6:
        {
            show_universe();
            break;
        }
        default:
            break;
        }

        cout << "Enter 0 to exit user view:\n";
        cout << "Enter 1 to register traveller :\n";
        cout << "Enter 2 to book ticket:\n";
        cout << "Enter 3 to update ticket:\n";
        cout << "Enter 4 to cancel ticket:\n";
        cout << "Enter 5 to view travellers travel detail:\n";
        cout << "Enter 6 to view universe:\n";
        cin >> i;
    }
}

int main()
{
    // code to get the current date from the os
    time_t tmNow;
    tmNow = time(NULL);
    struct tm t = *localtime(&tmNow);
    string G_TIME;
    G_TIME = to_string(t.tm_mday) + "/" + to_string(t.tm_mon + 1) + "/" + to_string(t.tm_year + 1900);
    gtime = (t.tm_year + 1900) * 365 + (float)(t.tm_mon + 1) * 30 + ((float)(t.tm_mday));
    cout << "Welcome to Space Travel Solutions :)\n";
    cout << "Current Date: " << G_TIME << "\n";

    // creating the universe with required number of passenger , astronaut and commander
    create_universe();

    // asking the user to enter as admin or user
    int i = user_input();
    while (i)
    {
        switch (i)
        {
        case 1:
        {
            user_access();
            break;
        }
        case 2:
        {
            admin_access();
            break;
        }
        default:
        {
            ;
        }
        }

        i = user_input();
    }
    cout << " THANK YOU, VISIT AGAIN :-) \n ";
    return 0;
}

/*
User Interface information -
it has 2 modes either to enter as a user or admin

admin has access to :
    > add planets
    > view universe
    > view traveller details of the universe

user has access to :
    >add traveller
    >book ticket
    >update ticket
    >cancel ticket
    >view universe
    >view travellers

*/