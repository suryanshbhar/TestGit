/*

Compiler Construction Lab Assignment 1

Suryansh Bhardwaj
101983044   COE20

*/

#include<bits/stdc++.h>
using namespace std;

// Given Grammar : (a+b)*abb

// links are the transitions corresponding to the thomson construction
// of the given grammmar.
// States have been named from 0 to 10


//    on edge         a , b , E1, E2
int links[11][4] = { {-1, -1, 1,  7}, 
                     {-1, -1, 2,  4}, 
                     {3 , -1, -1,-1},
                     {-1, -1,  6,-1},
                     {-1,  5, -1,-1},
                     {-1, -1,  6,-1},
                     {-1, -1,  1, 7},
                     {8,  -1, -1,-1},
                     {-1,  9, -1,-1},
                     {-1, 10, -1,-1},
                     {-1, -1, -1,-1} };


map<pair< set<int>,char > ,set<int>> transitions;
int start = 0;
int final = 10;



set<int> E_closure(int state){

    queue<int> q;
    q.push(state);
    set<int> closure;
    closure.insert(state);

    while(q.empty() == false){

        int t = q.front();
        q.pop();

        if(t == -1){
            continue;
        }

        int a = links[t][2];
        int b = links[t][3];

        if(closure.find(a)==closure.end() and a!=-1){
            closure.insert(a);
            q.push(a);

        }

         if(closure.find(b)==closure.end() and b!=-1){
            closure.insert(b);
            q.push(b);

        }
    }

    return closure;

}

set<set<int>> states;

void transition(){
    auto closure = E_closure(0);
    queue<set<int>> q;
    q.push(closure);
  

    while(q.empty() == false){
        auto closure = q.front();
        q.pop();
        states.insert(closure);

        // a transition
        set<int> t;

        for(auto a:closure){

            if(links[a][0] != -1){
                t.insert(links[a][0]);
            }

        }

        set<int> s;

        for(auto a:t){

            auto m = E_closure(a);

            for(auto j:m){

                if(s.find(j) == s.end()){
                    s.insert(j);
                }

            }

        }

        if(states.find(s)==states.end() ){
            states.insert(s);
            q.push(s);
        }

        transitions[make_pair(closure,'a')] = s;

        // b transition
         t.clear();

        for(auto a:closure){

            if(links[a][1] != -1){
                t.insert(links[a][1]);
            }

        }

        s.clear();


        for(auto a:t){

            auto m = E_closure(a);

            for(auto j:m){

                if(s.find(j) == s.end()){
                    s.insert(j);
                }

            }

        }

        if(states.find(s)==states.end() ){
            states.insert(s);
            q.push(s);
        }

        transitions[{closure,'b'}] = s;

    }
}



void verify( string input,map<pair<string,char>,string> dfa , string start_state , set<string> final_states ){
    cout<<endl;
    cout<<"Validating the input string :: "<<input<<endl;
    cout<<endl;

    string state = start_state;

    for(int i=0;i<input.size();i++){

        if( dfa.find(make_pair(state,input[i])) != dfa.end() ){

             cout<<"Current Symbol: "<<input[i]<<"  Current DFA state: "<<state<<"  {"<<state<<" on "<<input[i]<<"} --------->  "<<dfa[make_pair(state, input[i])]<<endl;
             state = dfa[make_pair(state, input[i])];

        }else{
            cout<<endl<<"INVALID STRING"<<endl;
            return;
        }


    }



    if(final_states.find(state) != final_states.end() ){

        cout<<endl<<"ACCEPT "<<endl;

    }else{
        cout<<endl<<"NOT ACCEPTED"<<endl;
    }




}

void printdfa(map<pair<string,char>,string> &dfa, string &start_state , set<string> final_states){
    for(auto key:dfa){ 

        cout<<key.first.first<<" on "<<key.first.second<<" ----------> "<<key.second<<endl;
      
    }

    cout<<"start_state: ";

    
     cout<<start_state<<endl;

    cout<<"final_states: ";

    for(auto a:final_states){
        cout<<a<<" ";

    }
    cout<<endl<<endl;
}




void minimize(map<pair<string,char>,string> &dfa , string &start_state , set<string> final_states,vector<string> names){

    cout<<"Minimizing the dfa: "<<endl<<endl;

    set<set<string>> min;

    
    set<string> others;

    for(auto a:names){

        if( final_states.find(a) == final_states.end() ){
            others.insert(a);
        }

    }
    min.insert(others);
    min.insert(final_states);

    set<set<string>> prev;





    while(prev!=min){

        cout<<"{ ";
        for(auto a:min){

            if(a.empty() == true){
                continue;
            }

            cout<<"( ";

            for(auto b:a){

                cout<<b<<" ";

            }

            cout<<" ), ";

        }
        cout<<" }"<<endl;


    set<set<string>> tbd;
    set<set<string>> tba;


    for(auto a:min){


        if(a.size() == 1){
            continue;
        }

        set<string> hat;

        for(auto b:a){

            string one = dfa[make_pair(b,'a')];
            string two = dfa[make_pair(b,'b')];

            if( a.find(one) == a.end() or a.find(two) == a.end() ){

                hat.insert(b);
            }

        }

        set<string> op = a;

        for(auto el:hat){
             op.erase(el);
        }

        tbd.insert(a);
        tba.insert(op);
        tba.insert(hat);
               
    }

    prev = min;


    for(auto f: tbd){

        min.erase(f);

    }

    for(auto f:tba){

        min.insert(f);

    }

}


cout<<endl<<"States after Minimizing : ";
for(auto a:min){
    if(a.empty() == true){
        continue;
    }

    for(auto b:a){

        cout<<b;

    }

    cout<<" , ";

}

cout<<endl;


for(auto a: min){

    if( a.size()>1 ){
        bool flag = 0;

        string t1 ;
        string t2 ;

        string new_name = "s_";

        for(auto b:a){

            if(b == start_state){
                flag = 1;

            }

            new_name+=b[2];

            t1 = dfa[make_pair(b,'a')];
            t2 = dfa[make_pair(b,'b')];

            dfa.erase(make_pair(b,'a'));
            dfa.erase(make_pair(b,'b'));
        }


        dfa[make_pair(new_name,'a')] = t1;
        dfa[make_pair(new_name,'b')] = t2;
        if(flag == 1){
             start_state = new_name;
        }
       
    }

}


    
}




void create_graph(string input){

    transition();

    map<set<int> , string> nodes;

    std::vector<string> names;

    int i=0;
    for(auto a:states){

        string name = "s_"+to_string(i);
        names.push_back(name);
        nodes[ a ] = name;
        i++;

    }

    set<string> final_states;
    string start_state="";

    for(auto a:states){
        for(auto j:a){

            if(j == final){
                final_states.insert(nodes[a]);
                break;

            }

        }

    }


    for(auto a:states){
        for(auto j:a){

            if(j == start){
                start_state = nodes[a];
                break;

            }

        }

    }

    map<pair<string,char>,string> dfa;

    for(auto f:transitions){

        auto key = f.first;


        string parent = nodes[key.first];
        char edge = key.second;

        string child = nodes[transitions[key]];

        dfa[make_pair(parent,edge)] = child;

    }

    cout<<endl;

    cout<<"The edges of DFA formed :";
    cout<<endl<<endl;

    printdfa(dfa,start_state,final_states);

    minimize(dfa,start_state,final_states,names);

    cout<<endl;
    cout<<"Minimized DFA :";
    cout<<endl<<endl;
    printdfa(dfa,start_state,final_states);


    verify(input,dfa,start_state,final_states);

}




int main() {
    // #ifndef ONLINE_JUDGE
    //   freopen("input.txt","r",stdin);
    //   freopen("out.txt","w",stdout);  
    // #endif

    cout<<"Please input a string which you want to verify from the given grammar :";
    string input;
    cin>>input;
    cout<<" "<<input<<endl;


    create_graph(input);


return 0;

}
