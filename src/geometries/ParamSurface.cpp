#include "ParamSurface.h"

ParamSurface::ParamSurface() {
    nVtxPerStrip = nStrips = 0;
}

VertexData ParamSurface::GenVertexData(float u, float v) {
    VertexData vtxData;
    vtxData.texcoord = vec2(u, v);
    Dnum2 X, Y, Z;
    Dnum2 U(u, vec2(1, 0)), V(v, vec2(0, 1));
    eval(U, V, X, Y, Z);
    vtxData.position = vec3(X.f, Y.f, Z.f);
    vec3 drdU(X.d.x, Y.d.x, Z.d.x), drdV(X.d.y, Y.d.y, Z.d.y);
    vtxData.normal = cross(drdU, drdV);
    return vtxData;
}

void ParamSurface::create(int N, int M) {
    nVtxPerStrip = (M + 1) * 2;
    nStrips = N;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= M; j++) {
            vtxData.push_back(GenVertexData((float) j / (float) M, (float) i / (float) N));
            vtxData.push_back(GenVertexData((float) j / (float) M, (float) (i + 1) / (float) N));
        }
    }
    glBufferData(GL_ARRAY_BUFFER, nVtxPerStrip * nStrips * sizeof(VertexData), &vtxData[0], GL_STATIC_DRAW);
    // Enable the vertex attribute arrays
    glEnableVertexAttribArray(0);  // attribute array 0 = POSITION
    glEnableVertexAttribArray(1);  // attribute array 1 = NORMAL
    glEnableVertexAttribArray(2);  // attribute array 2 = TEXCOORD0
    // attribute array, components/attribute, component type, normalize?, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) offsetof(VertexData, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) offsetof(VertexData, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) offsetof(VertexData, texcoord));
}

void ParamSurface::Draw() {
    glBindVertexArray(vao);
    for (unsigned int i = 0; i < nStrips; i++) glDrawArrays(GL_TRIANGLE_STRIP, i * nVtxPerStrip, nVtxPerStrip);
}

VertexData ParamSurface::GetVertexDataByUV(float u, float v) {
    return GenVertexData(u, v);
}

Sphere::Sphere() {
    create(tessellationLevel, tessellationLevel);
}

void Sphere::eval(Dnum2 &U, Dnum2 &V, Dnum2 &X, Dnum2 &Y, Dnum2 &Z) {
    U = U * 2.0f * (float) M_PI;
    V = V * (float) M_PI;

    X = Cos(U) * Sin(V);
    Y = Sin(U) * Sin(V);
    Z = Cos(V);
}