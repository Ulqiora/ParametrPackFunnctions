#include "MyMetaFunctions/type_pack.hpp"

int main(){
    static_assert(find<int>(type_pack<int,double*,char>{}) == 0);
    return 0;
}