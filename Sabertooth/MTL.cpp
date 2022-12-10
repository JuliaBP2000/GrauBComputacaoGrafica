#include "MTL.h"

MTL::MTL()
{
}

void MTL::createMtlFile()
{
	static ofstream mtlFile;
	mtlFile.open("../objs/pista.mtl");
	mtlFile << "newmtl " << "pista\n" << endl;
	mtlFile << "Kd " << 1.0 << " " << 0.5 << " " << 0 << endl;
	mtlFile << "Ka " << 1.0 << " " << 1.0 << " " << 1.0 << endl;
	mtlFile << "Tf " << 1.0 << " " << 1.0 << " " << 1.0 << endl;
	mtlFile << "map_Kd " << "../objs/pista.png" << endl;
	mtlFile << "Ni " << 1.0 << endl;
	mtlFile << "Ns " << 100.0 << endl;
	mtlFile << "Ks " << 1.0 << " " << 1.0 << " " << 1.0 << endl;
	mtlFile.close();
}