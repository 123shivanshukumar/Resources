#include<iostream>
#include<string>
#include<iomanip>
#include<vector>
#include<algorithm>
class comparator;
class Event{
    protected:
    int startTime, endTime;

    public:
    virtual double calculateProfit()=0;
    virtual void take_input()=0; // to take common input throughout 

    friend std::istream& operator>>(std::istream& stream, Event* e){
        e->take_input();
        return stream;
    }
    friend class comparator;

    friend bool non_overlapping(Event* a, Event*b){ // assuming b has higher finish time 
        if(a->endTime == b->endTime){return false;} // will definitely overlap
        return a->endTime <= b->startTime; // as per condition
    }
};
class comparator{
    public:
    comparator(){}
    bool operator()(Event* a, Event* b){
        return a->endTime < b->endTime;
    }
};
class Concert:public Event{
    int TicketPrice, TicketsSold,ArtisteFee,LogisticCost;
    
    void take_input(){
        std::cin>> startTime >> endTime >> TicketPrice >> TicketsSold >> ArtisteFee >> LogisticCost;
    }

    public:
    double calculateProfit(){
        double TicketRevenue = (double) (TicketPrice * TicketsSold);
        TicketRevenue = TicketRevenue - (0.18)*TicketRevenue;
        double Cost  = (double )ArtisteFee + LogisticCost;

        double profit = TicketRevenue - Cost;
        return (profit>2*Cost)?0.7*profit:profit;
    }
    

};

class TheaterShow:public Event{
    int BasePrice, TotalSeats, VenueCost;
    void take_input(){
        std::cin>> startTime >> endTime >> BasePrice >> TotalSeats >> VenueCost;
    }
    public:
    double calculateProfit(){
        double TicketRevenue = (double) 0.25 * TotalSeats * (2 * BasePrice) + (double) 0.75 * TotalSeats * BasePrice;
        TicketRevenue = TicketRevenue - (0.18)*TicketRevenue;
        
        double PopCornPrice = 150;
        double PopCornRevenue = ((double) 0.25*TotalSeats)*PopCornPrice;
        return TicketRevenue + PopCornRevenue - (double) VenueCost;
    }
};
class Wedding:public Event{
    int BaseAmount, DecorationCost, GuestCount, VenueCost;
    void take_input(){
        std::cin >> startTime >> endTime >> BaseAmount >> DecorationCost >> GuestCount >> VenueCost;
    }
    public:
    double calculateProfit(){
        double TotalVenueCost = (double) (GuestCount > 200) ? VenueCost*3 : VenueCost;
        double CateringCost = (double) (GuestCount > 100) ? GuestCount * 70 :  GuestCount * 100;
        return (double) BaseAmount - TotalVenueCost - (double) DecorationCost -  CateringCost;
    }
};

class EventScheduler{
    std::vector<Event*> events;
    public:
    friend std::istream& operator>>(std::istream & stream, EventScheduler& ev){
        int n;
        std::cin>>n;

        unsigned int type;
        while(n--){
            std::cin>> type;
            Event* e;
            if(type == 1){
                e = new Concert;  
            }
            else if (type == 2){
                e = new TheaterShow;
            }
            else if(type == 3){
                e = new Wedding;
            }
            std::cin>>e; // depending on run time type
            ev.events.push_back(e);
           
        }
        return stream;
    }
    double max_profit_non_overlapping(){
        
        std::sort(events.begin(), events.end(), comparator());
        
        int n = events.size();
        std::vector<double> dp(n,0); // dp of profits
        dp[0] = events[0]->calculateProfit();
        //std::cout<<events[0]->calculateProfit()<<" ";
        for(int j = 1; j < n; j++)
        {      
            double non_including = 0;
            for(int i = j - 1 ; i >= 0 ; i--){ // search for the first non overlapping interval before this and set the non_including to this value
                if(non_overlapping(events[i], events[j])){
                    non_including = dp[i];
                    break;
                }
            }
            dp[j] = std::max(dp[j-1], non_including + events[j]->calculateProfit()); // recursion 
            //std::cout<<events[j]->calculateProfit()<<" ";
        }

        return dp[n-1]; // final answer
    }
};
int main(){
    EventScheduler ev;
    std::cin>>ev;
    std::cout << std::fixed << std::setprecision(2) << ev.max_profit_non_overlapping() << "\n";
}