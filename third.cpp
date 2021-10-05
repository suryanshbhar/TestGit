#include<bits/stdc++.h>
using namespace std;

int links[11][4] = { {-1,-1,1,7} , 
                     {-1,-1,2,4} , 
                     {3,-1,-1,-1},
                     {-1,-1,6,-1},
                     {-1,5,-1,-1},
                     {-1,-1,6,-1},
                     {-1,-1,1,7} ,
                     {8,-1,-1,-1},
                     {-1,9,-1,-1},
                     {-1,10,-1,-1},
                     {-1,-1,-1,-1} };


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

        cout<<endl<<"STRING VALID"<<endl;

    }else{
        cout<<endl<<"INVALID STRING"<<endl;
    }




}


void create_graph(string input){

    transition();

    map<set<int> , string> nodes;

    int i=0;
    for(auto a:states){

        string name = "s_"+to_string(i);
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

    verify(input,dfa,start_state,final_states);

}






int main() {
    #ifndef ONLINE_JUDGE
      freopen("input.txt","r",stdin);
      freopen("out.txt","w",stdout);  
    #endif

    cout<<"Please input a string which you want to verify from the given grammar :";
    string input;
    cin>>input;


    create_graph(input);


return 0;

}