void simpleVertexPass() {
    if (renderBorder != 1) {
        if (useAdditionMatrix == 1) {
            gl_Position = viewProjection * additionMatrix * modelMatrix * position;
        } else {
            gl_Position = viewProjection * modelMatrix * position;
        }
    }
}
