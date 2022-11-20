#include <Kore/Graphics4/Graphics.h>
#include <Kore/Graphics4/PipelineState.h>
#include <Kore/Graphics4/Shader.h>
#include <Kore/IO/FileReader.h>
#include <Kore/System.h>

#include <limits>
#include <stdlib.h>

#include "Sprite.h"

#include "math.h"
#include "Input.h"

using namespace Kore;
// using namespace Engine;

namespace {
	Graphics4::Shader *vertexShader;
	Graphics4::Shader *fragmentShader;
	Graphics4::PipelineState *pipeline;
	Graphics4::VertexBuffer *vertices;
	Graphics4::IndexBuffer *indices;

	void update() {
		drawSprite(1,2,3);

		Graphics4::begin();
		Graphics4::clear(Kore::Graphics4::ClearColorFlag);

		Graphics4::setPipeline(pipeline);
		Graphics4::setVertexBuffer(*vertices);
		Graphics4::setIndexBuffer(*indices);
		Graphics4::drawIndexedVertices();

		Graphics4::end();
		Graphics4::swapBuffers();
		
		if (Engine::Input::keysDown[KeyW]) {
			printf("W\n");
		}
		if (Engine::Input::keysDown[KeyA]) {
			printf("A\n");
		}
		if (Engine::Input::keysDown[KeyS]) {
			printf("S\n");
		}
		if (Engine::Input::keysDown[KeyD]) {
			printf("D\n");
		}
	}
}

void drawRect(
	float x,
	float y,
	float w,
	float h,
	float *vertexBuffer,
	int *vertexIndex,
	int *indexBuffer,
	int *indexIndex
) {
	int VERTEX_SIZE = 3;
	int startIndex = floor((*vertexIndex) / VERTEX_SIZE);

	// top-left
	vertexBuffer[(*vertexIndex)++] = x;
	vertexBuffer[(*vertexIndex)++] = y;
	vertexBuffer[(*vertexIndex)++] = 0.5;

	// top-right
	vertexBuffer[(*vertexIndex)++] = x + w;
	vertexBuffer[(*vertexIndex)++] = y;
	vertexBuffer[(*vertexIndex)++] = 0.5;

	// bottom-left
	vertexBuffer[(*vertexIndex)++] = x;
	vertexBuffer[(*vertexIndex)++] = y + h;
	vertexBuffer[(*vertexIndex)++] = 0.5;

	// bottom-right
	vertexBuffer[(*vertexIndex)++] = x + w;
	vertexBuffer[(*vertexIndex)++] = y + h;
	vertexBuffer[(*vertexIndex)++] = 0.5;


	indexBuffer[(*indexIndex)++] = startIndex+0;
	indexBuffer[(*indexIndex)++] = startIndex+1;
	indexBuffer[(*indexIndex)++] = startIndex+2;

	indexBuffer[(*indexIndex)++] = startIndex+1;
	indexBuffer[(*indexIndex)++] = startIndex+3;
	indexBuffer[(*indexIndex)++] = startIndex+2;
}

int kickstart(int argc, char **argv) {
	Kore::System::init("Shader", 1024, 768);
	Kore::System::setCallback(update);

	FileReader vs("shader.vert");
	FileReader fs("shader.frag");
	vertexShader = new Graphics4::Shader(vs.readAll(), vs.size(), Graphics4::VertexShader);
	fragmentShader = new Graphics4::Shader(fs.readAll(), fs.size(), Graphics4::FragmentShader);
	Graphics4::VertexStructure structure;
	structure.add("pos", Graphics4::Float3VertexData);
	pipeline = new Graphics4::PipelineState();
	pipeline->inputLayout[0] = &structure;
	pipeline->inputLayout[1] = nullptr;
	pipeline->vertexShader = vertexShader;
	pipeline->fragmentShader = fragmentShader;
	pipeline->compile();

	// drawSprite(1,5,5,v,i)

	vertices = new Graphics4::VertexBuffer(8, structure);
	indices = new Graphics4::IndexBuffer(12);
	float *v = vertices->lock();
	int *i = indices->lock();
	
	int vertexIndex = 0;
	int indexIndex = 0;
	drawRect(-0.5, -0.5, 0.5, 0.5, v, &vertexIndex, i, &indexIndex);
	drawRect(0.5, 0.5, 0.5, 0.5, v, &vertexIndex, i, &indexIndex);

	vertices->unlock();
	indices->unlock();
	Engine::Input::init();

	Kore::System::start();

	return 0;
}
