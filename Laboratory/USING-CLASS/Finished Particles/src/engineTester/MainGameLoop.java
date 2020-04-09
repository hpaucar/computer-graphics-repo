package engineTester;

import org.lwjgl.opengl.Display;

import entities.Camera;
import models.ParticlesVao;
import particles.ParticleSystem;
import renderEngine.DisplayManager;
import renderEngine.Loader;
import renderEngine.Renderer;

public class MainGameLoop {

	public static void main(String[] args) {

		DisplayManager.createDisplay();
		Loader loader = new Loader();
		Renderer renderer = new Renderer();

		
		ParticlesVao vao = ParticlesVao.create(loader);
		
		Camera camera = new Camera();
		
		ParticleSystem particles = new ParticleSystem();
		
		while(!Display.isCloseRequested()){
			camera.move();
			particles.update();
			vao.store(particles.getParticles());
			renderer.prepare();
			renderer.render(vao, camera);
			DisplayManager.updateDisplay();
		}

		renderer.cleanUp();
		loader.cleanUp();
		DisplayManager.closeDisplay();

	}

}
