// CG5528.cpp
// 2023/02/07
// 2023年度CG講義総合課題
// 5528
// 舘澤 大門

#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#include <freeglut.h>

#include <GL/GL.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <Windows.h>
#include <random>
#include <unordered_map>

#include "model.h"
#include "texture.h"

static constexpr double PI = 3.1415926;
static constexpr int TEX_HEIGHT = 512;
static constexpr int TEX_WIDTH = 512;

PMXModelData pmx_data{};
std::vector<std::string> texture_paths = {
	"Tex\\skin.txt",
	"spa_skin_f.txt",
	"toon_clr.txt",
	"Tex\\EX_eyebase.txt",
	"Tex\\2nd_eyes.txt",
	"Tex\\exp.txt",
	"spa_eyes.txt",
	"Tex\\outerC.txt",
	"spa_gn.txt",
	"toon_blgr.txt",
	"spa_skin.txt",
	"toon_skin.txt",
	"Tex\\outerA.txt",
	"spa_lf.txt",
	"Tex\\outerB.txt",
	"toon_hr.txt",
	"spa_bt.txt",
	"spa_sh.txt",
	"toon_sh.txt",
	"spa_sk.txt",
	"toon_skf.txt",
	"spa_amc.txt",
	"spa_hr.txt",
	"Tex\\hair.txt"
};
std::unordered_map<std::string, OriginalTexture> textures;

int viewportWidth, viewportHight;
void resize(int w, int h);
void display(void);
void idle(void); // アイドル中の再描画
void myAxis(void);
// インターフェース,キーボード,マウス操作
bool flagWire = true;
void keyboard(unsigned char key, int x, int y);

int fix_eyes_index = 7;
float view_radius = 100.0;
float fix_eyes[8][3];

void myInit();
void myLight01(void);
void triangle(std::vector<Float3>);
void drawPMX(const PMXModelData&);
void setView();
void displayTexPolygon(void);

int main(int argc, char** argv) {
	
	if (!loadPMX(pmx_data, "C:\\Users\\toida\\source\\repos\\OpenGLTest\\OpenGLTest\\hatsunemiku.pmx")) {
		printf("Load PMX failed");
		return -1;
	}
	printf("Load PMX success\n");

	for (int pi = 0; pi < pmx_data.texturePaths.size(); pi++) {
		pmx_data.texturePathsString.push_back(texture_paths[pi]);
		textures[texture_paths[pi]] = OriginalTexture{};
		textures[texture_paths[pi]].init("C:\\Users\\toida\\source\\repos\\OpenGLTest\\OpenGLTest\\miku\\" + texture_paths[pi]);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("test");
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);//キーボードの利用
	glutIdleFunc(idle); //再描画
	glClearColor(0.5, 0.5, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glutIdleFunc(idle);

	myInit();
	setView();

	glutMainLoop();
	return 0;
}

void setView() {
	for (int i = 0; i < 8; i++) {
		fix_eyes[i][0] = view_radius * cos(i * PI / 4);
		fix_eyes[i][1] = view_radius * sin(i * PI / 4);
		fix_eyes[i][2] = view_radius / 2;
	}
}

void myInit() {
	glClearColor(0.5, 0.5, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	myLight01();
}

void myLight01(void)
{
	// 光源
	GLfloat light0_pos[] = { 5.0, -2.0, 10.0, 1.0 }; /* 光源位置 */
	GLfloat light0_amb[] = { 0.1, 0.1, 0.1, 0.0 }; /* 環境光の強さ,色 */
	GLfloat light0_dif[] = { 1.0, 1.0, 1.0, 0.0 }; /* 拡散光の強さ,色 */
	GLfloat light0_spc[] = { 0.8, 0.8, 0.8, 0.0 }; /* 鏡面反射光の強さ,色 */
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_spc);
	//glShadeModel(GL_FLAT); // GL_FLAT or GL_SMOOTH
	glEnable(GL_LIGHTING); //反射物体のための光源発光
	glEnable(GL_LIGHT0);
}

void resize(int w, int h) {
	viewportWidth = w;
	viewportHight = h;
	// ビューポート変換U
	glViewport(0, 0, w, h); //ウインドウ全体に表示
	// 投影変換P
	glMatrixMode(GL_PROJECTION); // 投影変換P モードに切替
	glLoadIdentity();
	// 選択: 以下2のどちらかを選択
	// 1)透視投影 // 視野角のある通常光学系
	gluPerspective(10.0, (double)viewportWidth / (double)viewportHight, 1.0, 1000.0);
	// 2)正射投影 // テレセントリック
	//glOrtho(-1.0, 1.0, -1.0, 1.0, 3.0, 7.0);
}

void display(void) {
	//カラーバッファ,デプスバッファのクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//変更
	// 視野変換V
	glMatrixMode(GL_MODELVIEW); //モデル変換モードの開始
	glLoadIdentity(); //変換行列の初期化
	//gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	gluLookAt(fix_eyes[fix_eyes_index][0], fix_eyes[fix_eyes_index][1], fix_eyes[fix_eyes_index][2], 0.0, 0.0, 0.5, 0.0, 0.0, 1.0);

	glDisable(GL_LIGHTING); //反射物体のための光源発光
	glDisable(GL_LIGHT0);
	myAxis();
	glEnable(GL_LIGHTING); //反射物体のための光源発光
	glEnable(GL_LIGHT0);

	drawPMX(pmx_data);

	glutSwapBuffers();
	//glFlush();
}

void idle(void) {
	glutPostRedisplay(); //再描画
}

void keyboard(unsigned char key, int x, int y) {
	switch ((unsigned char)key) {
	case 'd':
		fix_eyes_index++;
		break;
	case 'a':
		fix_eyes_index--;
		break;
	case 'w':
		view_radius -= 10;
		break;
	case 's':
		view_radius += 10;
		break;
	default:
		break;
	}

	if (view_radius < 0) {
		view_radius = 0;
	}

	if (fix_eyes_index > 7) {
		fix_eyes_index = 0;
	}
	else if (fix_eyes_index < 0) {
		fix_eyes_index = 7;
	}

	setView();
}

void myAxis(void) {
	glLineWidth(1.0); //線幅
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //塗りつぶしは GL_FILL
	glBegin(GL_LINES);
	//x軸の描画
	glColor3f(1.0, 0.0, 0.0); //線の色
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.9, 0.0, 0.0);
	//y軸の描画
	glColor3f(0.0, 1.0, 0.0); //線の色
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.9, 0.0);
	//z軸の描画
	glColor3f(0.0, 0.0, 1.0); //線の色
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.9);
	glEnd();
}

void triangle(std::vector<Float3> verteces, std::vector<Float3> normal, std::vector<Float2> uv, Material mat) {
	GLfloat m3_amb[] = { mat.ambient.x, mat.ambient.y, mat.ambient.z, 1.0 };
	GLfloat m3_dif[] = { mat.diffuse.x, mat.diffuse.y, mat.diffuse.z, mat.diffuse.w };
	GLfloat m3_spc[] = { mat.specular.x, mat.specular.y, mat.specular.z, 1.0 };
	GLfloat m3_shi[] = { mat.specularity };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m3_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m3_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m3_spc);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, m3_shi);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	for (int i = 2;i >= 0;i--) {
		glTexCoord2f(uv[i].x, uv[i].y);
		glNormal3f(normal[i].y, normal[i].x, normal[i].z);
		glVertex3f(verteces[i].y, verteces[i].x, verteces[i].z);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawPMX(const PMXModelData& data) {
	glTranslatef(0.0, 0.0, -10.0);
	glRotated(-90.0, 0.0, 1.0, 0.0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	uint32_t last_index = 0;
	for (int i = 0; i < data.materials.size(); i++) {
		auto image = textures[texture_paths[data.materials[i].colorMapTextureIndex]].image;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_WIDTH, TEX_HEIGHT,
			0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);

		uint16_t triangle_length = data.materials[i].vertexNum / 3;
		for (int j = last_index; j < (last_index + triangle_length); j++) {
			std::vector<Float3> tri_verteces{};
			std::vector<Float3> tri_normals{};
			std::vector<Float2> tri_uvs{};
			for (int k = 0; k < 3; k++) {
				uint16_t vertex_index = data.surfaces[j * 3 + k].vertexIndex;
				tri_verteces.push_back(data.vertices[vertex_index].position);
				tri_normals.push_back(data.vertices[vertex_index].normal);
				tri_uvs.push_back(data.vertices[vertex_index].uv);
			}
			triangle(tri_verteces, tri_normals, tri_uvs, data.materials[i]);
		}
		last_index += triangle_length;
	}
}

void displayTexPolygon(void)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	auto image = textures["Tex\\2nd_eyes.txt"].image;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_WIDTH, TEX_HEIGHT,
		0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -5.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -5.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}