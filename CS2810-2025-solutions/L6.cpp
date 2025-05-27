#include<bits/stdc++.h>
using namespace std;
const long long mod=1e9+7;

class CurrencySorter 
{
	public:
	
	int partition(vector<int>& arr, int low, int high) {
	    int pivot = arr[high]; 
	    int i = low - 1;

	    for (int j = low; j < high; j++) {
	        if (arr[j] < pivot) {
	            i++;
	            swap(arr[i], arr[j]);
	        }
	    }
	    swap(arr[i + 1], arr[high]);
	    return i + 1;
	}

	int randomizedPartition(vector<int>& arr, int low, int high) {
	    int randomIndex = low + rand() % (high - low + 1);
	    swap(arr[randomIndex], arr[high]);
	    return partition(arr, low, high);
	}

	void quicksort(vector<int>& arr, int low, int high) {
	    if (low < high) {
	        int pivotIndex = randomizedPartition(arr, low, high);
	        quicksort(arr, low, pivotIndex - 1);
	        quicksort(arr, pivotIndex + 1, high);
	    }
	}

	void quicksort(vector<int>& arr) {
	    quicksort(arr, 0, arr.size() - 1);
	}	
	
	void operator() (vector<int>& arr)
	{
		quicksort(arr);
	}
};

class FibonacciGenerator 
{
    public:
    vector<vector<long long>> matrixMultiply2D(vector<vector<long long>>& mat1,vector<vector<long long>>& mat2)
    {
        vector<vector<long long>> ans(2,vector<long long>(2,0));
        
        
        //below operations simulate 2d matrix multiplication
        ans[0][0]=(((mat1[0][0]*mat2[0][0])%mod)+((mat1[0][1]*mat2[1][0])%mod))%mod;
        ans[0][1]=(((mat1[0][0]*mat2[0][1])%mod)+((mat1[0][1]*mat2[1][1])%mod))%mod;
        ans[1][0]=(((mat1[1][0]*mat2[0][0])%mod)+((mat1[1][1]*mat2[1][0])%mod))%mod;
        ans[1][1]=(((mat1[1][0]*mat2[0][1])%mod)+((mat1[1][1]*mat2[1][1])%mod))%mod;
        
        return ans;
    }

    vector<vector<long long>> fibo(vector<vector<long long>>& mat,long long pow)
    {
        if(pow==0) return {{1,0},{0,1}};
        
        vector<vector<long long>> mid=fibo(mat,pow/2);
        
        mid=matrixMultiply2D(mid,mid);
        if(pow%2==1) mid=matrixMultiply2D(mid,mat);
        
        return mid;
    }
     
    long long operator() (long long n)
    {
        if(n<=1) return n;
        vector<vector<long long>> mat={{1,1},{1,0}};
        vector<vector<long long>> ans=fibo(mat,n-1);
        return ans[0][0];
    }   
};

class PrimeCalculator 
{
    public:
        static vector<bool> seive;
        static bool done;
        static vector<long long> primes;
        
        static void preComputePrimes()
        {
            seive[0]=seive[1]=false;
            
            for(long long i=2;i<=1e6;i++)
            {
                if(seive[i])
                {
                    primes.push_back(i);
                    for(long long j=i*i;j<=1e6;j+=i) seive[j]=false;
                }
            }
            
        }
        static void printPrimes(long long L,long long R)
        {
            if(!done)
            {
                preComputePrimes();
                done=true;
            }
            
            
            vector<bool> isPrime(R-L+1,true);
            
            for(long long i:primes)
            {
                for(long long j=max(i*i,(L+i-1) / i * i); j<=R;j+=i) isPrime[j-L]=false;
            }
        
            if(L==1) isPrime[0]=false;
            
            for(int i=0;i<isPrime.size();i++)
            {
                if(isPrime[i]) cout<<i+L<<" ";
            }
            cout<<"\n";
        }
        
        static long long printPrimeSum(long long L,long long R)
        {
            if(!done)
            {
                preComputePrimes();
                done=true;
            }
            
            vector<bool> isPrime(R-L+1,true);
            
            for(long long i:primes)
            {
                for(long long j=max(i*i,(L+i-1) / i * i); j<=R;j+=i) isPrime[j-L]=false;
            }
        
            if(L==1) isPrime[0]=false;
            
            long long sum=0;
            
            for(int i=0;i<isPrime.size();i++)
            {
                if(isPrime[i]) sum+=(i+L);
            }
            return sum;
        }
};

class NumberAnalyzer 
{
	public:
		
		static bool isSquareFree(int x)
		{
			//x would be >=2
			
			for(long long i=2;i*i<=x;i++)
			{
				long long sq=i*i;
				if(x%sq==0) return false;
			}
			return true;
		}
		
		static long long countDivisors(int x)
		{
			long long ans=0;
			
			for(long long i=1;i*i<=x;i++)
			{
				if(x%i==0)
				{
					ans++;
					long long rem=x/i;
					if(rem!=i) ans++;
				}
			}
			return ans;
		}
		
		static long long sumOfDivisors(int x)
		{
			long long sum=0;
			
			for(long long i=1;i*i<=x;i++)
			{
				if(x%i==0)
				{
					sum+=i;
					long long rem=x/i;
					if(rem!=i) sum+=rem;
				}
			}
			return sum;
		}
};

vector<bool> PrimeCalculator:: seive(1e6+1,true);
bool PrimeCalculator:: done=false;
vector<long long> PrimeCalculator:: primes;

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	
	int type; cin>>type;
	
	CurrencySorter currencySorter;
	FibonacciGenerator fibonacciGenerator;
	
	if(type==1)
	{
		int t;
		cin>>t;
		
		while(t--)
		{
			int n; cin>>n;
			vector<int> arr(n);
			
			for(int i=0;i<n;i++) cin>>arr[i];
			currencySorter(arr);
		
			for(auto a:arr) cout<<a<<" ";
			cout<<"\n";
		}
	}
	else if(type==2)
	{
		int t; cin>>t;
		
		while(t--)
		{
			long long n; cin>>n;
			cout<<fibonacciGenerator(n)<<"\n";
		}
	}
	else if(type==3)
	{
		int t; cin>>t;
		
		while(t--)
		{
			string command; cin>>command;
			long long L,R;  cin>>L>>R;
			
			if(command=="printPrimes")
			{
				PrimeCalculator::printPrimes(L,R);
			}
			else if(command=="printPrimeSum")
			{
				cout<<PrimeCalculator::printPrimeSum(L,R)<<"\n";
			}
		}
	}
	else if(type==4)
	{
		int t; cin>>t;
		
		while(t--)
		{
			string command; cin>>command;
			int x; cin>>x;
			
			if(command=="isSquareFree")
			{
				if(NumberAnalyzer::isSquareFree(x)) cout<<"yes\n";
				else cout<<"no\n";
			}
			else if(command=="countDivisors")
			{
				cout<<NumberAnalyzer::countDivisors(x)<<"\n";
			}
			else if(command=="sumOfDivisors")
			{
				cout<<NumberAnalyzer::sumOfDivisors(x)<<"\n";
			}
		}
	}
	return 0;
}