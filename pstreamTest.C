#include "fvCFD.H"

int main(int argc, char* argv[]){

    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
  //  argList::noCheckProcessorDirectories();
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

    // Add the values from all processes together
    Pout << "Mesh volume on this processor: " << meshVolume << endl;
    reduce(meshVolume, sumOp<scalar>());
    Info << "Total mesh volume on all processors: " << meshVolume
         << " over " << returnReduce(mesh.C().size(), sumOp<label>()) 
         << " cells" << endl;

    // Spreading a value across all processors with scatter operation.
    Pstream::scatter(meshVolume);
    Pout << "Mesh volume on this processor is now " << meshVolume << endl;

    // Gather and Scatter a List
    List<label> nInternalFaces (Pstream::nProcs()), nBoundaries (Pstream::nProcs());
    nInternalFaces[Pstream::myProcNo()] = mesh.Cf().size();
    nBoundaries[Pstream::myProcNo()] = mesh.boundary().size();

    // Gather the list on the root task
    Pstream::gatherList(nInternalFaces);
    Pstream::gatherList(nBoundaries);

    // Print the information of the list for each processor
    if (Pstream::master())
    {
        forAll(nInternalFaces,i)
            Info << "Processor " << i << " has " << nInternalFaces[i]
                 << " internal faces and " << nBoundaries[i] << " boundary patches" << endl;
    }

    // Scatter the list to all tasks
    Pstream::scatterList(nInternalFaces);
    Pstream::scatterList(nBoundaries);

    // As the mesh is decomposed, interfaces between processors are turned
    // into patches, meaning each subdomain sees a processor boundary as a
    // boundary condition.
    forAll(mesh.boundary(),patchI)
        Pout << "Patch " << patchI << " named " << mesh.boundary()[patchI].name() << endl;

    #include "createFields.H"
}

    
