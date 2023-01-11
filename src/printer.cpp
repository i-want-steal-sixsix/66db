#include<bits/stdc++.h>

//test
std::vector<std::vector<std::string>> test{{"col1","col2","col3"},{"12324","3123213","312"},{"312321","312321323"},{"我是卡莎！","我是莎莎！"}};

void print_head(const int &_max_len, const int &_col_nums){
  std::cout<<"+";
  for(int i=0;i<_col_nums;i++){
      for(int j=0;j<_max_len+2;j++){
        std::cout<<"=";
      }
      std::cout<<"+";
  }
  std::cout<<std::endl;
}

void print_records(const std::vector<std::vector<std::string>> &vec_outer){
      std::vector<std::string> vec_inner;
      std::string col;
      int max_len = 0;
      int col_nums = 0;
      for(int i=0;i<vec_outer.size();i++){
          vec_inner = vec_outer[i];
          if(i == 0) col_nums = vec_inner.size();
        for(int j=0;j<vec_inner.size();j++){
            col = vec_inner[j];
          if(col.size() > max_len){
              max_len = col.size();
          }
        }
      }
      print_head(max_len,col_nums);
      for(int i=0;i<vec_outer.size();i++){
          vec_inner = vec_outer[i];
          std::cout<<"|";
          for(int j=0;j<col_nums;j++){
              if(j<vec_inner.size()){
                col = vec_inner[j];
                std::cout<<col;
                for(int k= 0;k<(max_len+2-col.size());k++){
                    std::cout<<" ";
                }
                    std::cout<<"|";
              }
              else{
                for(int k= 0;k<(max_len+2);k++){
                    std::cout<<" ";
                }
                    std::cout<<"|";
              }
          }
          std::cout<<std::endl;
          if(i==0) print_head(max_len,col_nums);
          
      }
      print_head(max_len,col_nums);
}

int main(){
  print_records(test);
}