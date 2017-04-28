void simpleVertexPass(vec4 positionIn) {
    if (renderBorder != 1) {
        if (useAdditionMatrix == 1) {
            gl_Position = viewProjection * additionMatrix * modelMatrix * positionIn;
        } else {
            gl_Position = viewProjection * modelMatrix * positionIn;
        }
    }
}
