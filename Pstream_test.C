#include "fvCFD.H";

int main(int argc, char* argv[]){
    argList::noCheckProcessorDirectories();
    #include "setRootCase.H";
    if(!Pstream::parRun()){
        FatalErrorInFunction
        << ": This utility can only be run parallel"
        << exit(FatalError);
    }
Pout<< "Hello from process " << Pstream::myProcNo() << endl;

//calculate the totlal mesh volume
scalar meshVolume(0.);
forAll(mesh.V(), cellI){
    meshVolume += mesh.V()[cellI];
}
 
}

    