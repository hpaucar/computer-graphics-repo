package models;

import java.nio.FloatBuffer;
import java.util.List;

import org.lwjgl.BufferUtils;
import org.lwjgl.util.vector.Vector3f;

import particles.Particle;
import renderEngine.Loader;

public class ParticlesVao {
	
	private static final int MAX_PARTICLES = 1000;
	private static final FloatBuffer buffer = BufferUtils.createFloatBuffer(MAX_PARTICLES * 3);
	
	private int vao;
	private int vbo;
	private int particleCount = 0;
	
	private Loader loader;
	
	public ParticlesVao(int vaoID, int vboID, Loader loader){
		this.vao = vaoID;
		this.vbo = vboID;
		this.loader = loader;
	}
	
	public void store(List<Particle> particles){
		this.particleCount = Math.min(particles.size(), MAX_PARTICLES);
		float[] particleData = new float[particleCount * 3];
		for(int i=0;i<particleCount;i++){
			Vector3f pos = particles.get(i).getPosition();
			particleData[i*3] = pos.x;
			particleData[i*3 + 1] = pos.y;
			particleData[i*3 + 2] = pos.z;
		}
		loader.updateVbo(vbo, particleData, buffer);
	}

	public int getVaoID() {
		return vao;
	}

	public int getParticleCount() {
		return particleCount;
	}
	
	public static ParticlesVao create(Loader loader){
		return loader.createEmptyVAO(MAX_PARTICLES);
	}
	

}
