# Boids

*I've been reading some books on artificial life and self-organisation and decided to implement this simple simulation of birds in flight.*

Boids, as described by their creator, Craig Reynolds, represent bird-oid (or bird-like) objects that display flocking behaviour similar to that of birds.

This small programme I wrote using SFML and C++ simulates the movement of boids in two dimensions and abides by the three rules put forth by Reynolds in his 1987 paper titled [Flocks, herds and schools: A distributed behavioral model](https://dl.acm.org/doi/10.1145/37401.37406).

The three rules are as follows (a single boid is coloured in red to allow the viewer to isolate it from the flock):


1. Separation - each boid strives not to overcrowd their local flockmates by moving away from them gradually; the closer they are, the more rapidly they'll move away from one another. Each boid also has a field of view, or a radius of perception if you will, and anything beyond that circle does not concern the boid and it will not react to it.

![](gifs/separation.gif)

There is almost a law of conservation in play as the boids do not transfer any energy upon rebounding against the screen borders, and furthermore, the only time they speed up is when a neighbouring boid transfers some *repulsive force* onto them and the only time they slow down is when they repel other boids. The only initial force is applied to each boid at the very beginning giving them random velocity vectors. Under these circumstances, the boids have no sense of direction other than when trying to avoid other boids by going in the opposite direction. Not much going on, as can be observed on the gif above.

2. Alignment - each boid strives to align its own direction of movement with that of its local flockmates.

![](gifs/alignment.gif)

I've allowed the boids to wrap around the screen this time, as the *alignment *behaviour is a bit difficult to observe when they're confined to the screen borders. The movement of the entire flock is dependent upon the average starting velocity of all the boids; this dictates the overall cardinal direction they will all fall into very quickly after the simulation begins. Some boids are indeed spawned with starting velocities diametrically opposed to those of the entire flock, and it takes them a while before they fall in line, so to speak, and begin to align with the entire flock.

3. Cohesion - each boid strives to gravitate towards a *local centre of mass* that is the average position of its local flockmates.

![](gifs/cohesion.gif)

The flock seems to behave the same way as it did in the previous stage, but there's a small caveat; those few initial boids that are spawned with a velocity vector close to the inverse of the flock's velocity won't actually gradually align with the rest as this rule only takes positions into account, rather than directions. Therefore the *dissident* boids will likely fly against the tide in perpetuity, unless they come across a small flock with very high cohesion in which case they'll likely join them - I believe this happens once or twice in the gif above.

4. All the rules added together - I've also included a tiny central pulling force to keep the flock near the centre of the screen.

![](gifs/final.gif)

The boids now show clear flocking and steering behaviour that emerge from these three simple rules. By slighly modifying the parameters and the relative intensity of each rule, one can produce a variety of deviations to the expected flocking behaviour such as the one below:

![](gifs/variation.gif)

If you want to read more about Craig Reynolds's work here are the [slides](2016_UCSC_Steering_Behaviors.pdf) to his most recent lecture on steering behaviours from 2016.
