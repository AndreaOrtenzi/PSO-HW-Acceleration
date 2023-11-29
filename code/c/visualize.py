import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib.patches as patches


def cost_func_1(x, y):
    return x**2 + y**2


def cost_func_2(x, y):
    return 0.26*(x**2 + y**2) - 0.48*x*y


info = np.loadtxt('output.txt')
x = np.linspace(info[0,4], info[0,5], 100)
y = np.linspace(info[0,4], info[0,5], 100)

X, Y = np.meshgrid(x, y)
func_name = eval('cost_func_' + str(int(info[0,1])))
Z = func_name(X, Y)

fig, ax = plt.subplots(figsize=(9, 7))
contour = ax.contour(X, Y, Z, levels=80, cmap='gnuplot')

arrow_scale = 0.001

init_particles = info[info[:, 0] == 0]
scatter = ax.scatter(init_particles[:-2, 2], init_particles[:-2, 3], label='Particles')
scatter_swarm = ax.scatter(init_particles[-2, 2], init_particles[-2, 3], c='r', label='Swarm Best Position')
quiver = ax.quiver(init_particles[:-2, 2], init_particles[:-2, 3], init_particles[:-2, 4], init_particles[:-2, 5], angles='xy', width=0.002, headwidth=3, headlength=4)

iter_text = plt.text(0, -4.7, '', ha='center', fontsize=10, zorder=101)
iter_rect = patches.Rectangle((-1, -4.9), 2, 0.6, ec="gray", fc="white", alpha=0.8, zorder=100)
ax.add_patch(iter_rect)

score_text = plt.text(-4.7, -4.7, '', fontsize=10, zorder=101)
score_rect = patches.Rectangle((-4.9, -4.9), 3.4, 1.2, ec="gray", fc="white", alpha=0.8, zorder=100)
ax.add_patch(score_rect)


def update(i):
    particles = info[info[:, 0] == i]
    positions = particles[:-2, 2:4]
    # Update scatter plot data
    scatter.set_offsets(positions)
    scatter_swarm.set_offsets(particles[-2, 2:4])
    # Update quiver plot data
    quiver.set_offsets(positions)
    quiver.set_UVC(arrow_scale * particles[:-2, 4], arrow_scale * particles[:-2, 5])

    iter_text.set_text('Iteration: ' + str(int(i)))
    score_text.set_text(f'Best Score:\n{particles[-1, 2]}\nat ({particles[-2, 2]}, {particles[-2, 3]})')

    return scatter, scatter_swarm, quiver, iter_text, score_text

ani = FuncAnimation(fig, update, frames=int(info[0,3]), interval=100, blit=True)

plt.title(f'Particle Swarm Optimization: cost function {int(info[0, 1])}\nParticles: {int(info[0, 2])}  -  Iterations: {int(info[0,3])}')
plt.xlabel('x')
plt.ylabel('y')
plt.legend(loc='upper left')
plt.colorbar(contour)
plt.show()
# ani.save('animation.gif', writer='pillow', fps=10)