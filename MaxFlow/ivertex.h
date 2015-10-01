#ifndef IVERTEX
#define IVERTEX
#include "ivertexextension.h"

class IVertex {
public:
       virtual void initExtension(IVertexExtension* extension) = 0;
       virtual IVertexExtension* getExtension() const = 0;
       virtual ~IVertex() {}
};

#endif // IVERTEX

