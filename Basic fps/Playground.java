package Playground;

import com.jme3.app.SimpleApplication;
import com.jme3.bullet.BulletAppState;
import com.jme3.bullet.PhysicsSpace;
import com.jme3.bullet.collision.PhysicsCollisionEvent;
import com.jme3.bullet.collision.PhysicsCollisionListener;
import com.jme3.bullet.collision.shapes.CapsuleCollisionShape;
import com.jme3.bullet.collision.shapes.CollisionShape;
import com.jme3.bullet.collision.shapes.SphereCollisionShape;
import com.jme3.bullet.control.CharacterControl;
import com.jme3.bullet.control.RigidBodyControl;
import com.jme3.bullet.util.CollisionShapeFactory;
import com.jme3.collision.CollisionResults;
import com.jme3.effect.ParticleEmitter;
import com.jme3.effect.ParticleMesh;
import com.jme3.font.BitmapText;
import com.jme3.input.KeyInput;
import com.jme3.input.MouseInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.input.controls.MouseButtonTrigger;
import com.jme3.light.AmbientLight;
import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;
import com.jme3.math.FastMath;
import com.jme3.math.Plane;
import com.jme3.math.Quaternion;
import com.jme3.math.Ray;
import com.jme3.math.Vector2f;
import com.jme3.math.Vector3f;
import com.jme3.post.FilterPostProcessor;
import com.jme3.post.filters.FogFilter;
import com.jme3.renderer.RenderManager;
import com.jme3.renderer.queue.RenderQueue;
import com.jme3.scene.Geometry;
import com.jme3.scene.Node;
import com.jme3.scene.Spatial;
import com.jme3.scene.shape.Box;
import com.jme3.scene.shape.Quad;
import com.jme3.scene.shape.Sphere;
import com.jme3.util.SkyFactory;
import com.jme3.water.SimpleWaterProcessor;
import com.jme3.water.WaterFilter;

/**
 * test
 *
 * @author normenhansen
 */
public class Playground extends SimpleApplication implements ActionListener, PhysicsCollisionListener {

    /**
     * ** Global Variable Definition
     * *****************************************************
     */
    private Spatial mainScene;
    private WaterFilter water;
    private Vector3f lightDir = new Vector3f(-4.0f, -1.0f, 5.0f);
    private BulletAppState bulletAppState;
    private RigidBodyControl landscape;
    private CharacterControl player;
    private CharacterControl enemy;
    private Vector3f walkDir = new Vector3f();
    private boolean left = false, right = false, up = false, down = false;
    private Vector3f camDir = new Vector3f();
    private Vector3f camLeft = new Vector3f();
    private RigidBodyControl ball_phy;
    private static final Sphere ball;

    static {
        ball = new Sphere(32, 32, 0.1f, true, false);
        ball.setTextureMode(Sphere.TextureMode.Projected);
    }
    Material ball_mat;
    // For shooting
    private Node gnomes;
    Material black_mat;
    private RigidBodyControl box_phy;
    Geometry bg1;
    Geometry bg2;
    Geometry bg3;
    private SphereCollisionShape bulletCollisionShape;
    // For collecting
    private Node ingredients;
    Material red_mat;
    Geometry rg1;
    Geometry rg2;
    Geometry rg3;
    Geometry rg4;
    Geometry rg5;
    Geometry rg6;
    // Keeping the score
    int points = 0;
    int maxPoints = 21;
    BitmapText score;
    BitmapText ch;
    int hitsTroglodau = 0;
    int hitsProstalau = 0;
    int hitsDumbalau = 0;
    BitmapText txt; // game over
    //TODO: Enemy
    private Node enemyNode;

    /**
     * *** Main
     * ************************************************************************
     */
    public static void main(String[] args) {
        Playground app = new Playground();
        app.start();
    }

    /**
     * *** Initialize the application
     * **************************************************
     */
    @Override
    public void simpleInitApp() {

        bulletAppState = new BulletAppState();
        stateManager.attach(bulletAppState);
        viewPort.setBackgroundColor(new ColorRGBA(0.7f, 0.8f, 1f, 1f));
        bulletCollisionShape = new SphereCollisionShape(0.4f);

        getPhysicsSpace().addCollisionListener(this);

        flyCam.setMoveSpeed(100);

        cam.setLocation(new Vector3f(0, 4f, 6f));
        cam.lookAt(new Vector3f(2, 2, 0), Vector3f.UNIT_Y);

        keys();

        scene();
        materials();
        // cross();
        sky();
        light();
        //  simpleWater();
        water();
        //   fog(); // messes with the water

        spreadGnomes();
    }

    /**
     * *** Put the scene in place
     * *****************************************************
     */
    private void scene() {
        mainScene = assetManager.loadModel("Scenes/mainScene.j3o");
        mainScene.setLocalScale(2f);

        CollisionShape sceneShape =
                CollisionShapeFactory.createMeshShape((Node) mainScene);
        landscape = new RigidBodyControl(sceneShape, 0);
        mainScene.addControl(landscape);

        CapsuleCollisionShape capsuleShape = new CapsuleCollisionShape(1.5f, 6f, 1);
        player = new CharacterControl(capsuleShape, 0.05f);
        player.setJumpSpeed(20);
        player.setFallSpeed(30);
        player.setGravity(30);
        player.setPhysicsLocation(new Vector3f(0, 10, 0));

        rootNode.attachChild(mainScene);
        bulletAppState.getPhysicsSpace().add(landscape);
        bulletAppState.getPhysicsSpace().add(player);

        CapsuleCollisionShape enemyCapsuleShape = new CapsuleCollisionShape(1.5f, 6f, 1);
        enemy = new CharacterControl(enemyCapsuleShape, 0.05f);
        enemy.setJumpSpeed(20);
        enemy.setFallSpeed(30);
        enemy.setGravity(30);
        enemy.setPhysicsLocation(new Vector3f(20, 10, 0));
        enemyNode = new Node("enemy node");
        Spatial model = assetManager.loadModel("Models/Sinbad/Sinbad.mesh.xml");
        model.scale(0.25f);
        enemyNode.addControl(enemy);
        bulletAppState.getPhysicsSpace().add(enemy);
        rootNode.attachChild(enemyNode);
        enemyNode.attachChild(model);


        bulletAppState.getPhysicsSpace().add(landscape);
        bulletAppState.getPhysicsSpace().add(player);
        bulletAppState.getPhysicsSpace().add(enemy);

        // score stuff

        score = new BitmapText(guiFont, false);
        score.setSize(guiFont.getCharSet().getRenderedSize() * 2);
        score.setText("Score : " + points + "/" + maxPoints);
        score.setLocalTranslation(
                5 * settings.getWidth() / 7,
                settings.getHeight() / 4, 0);
        guiNode.attachChild(score);
    }

    /**
     * *** There will be light
     * *******************************************************
     */
    private void light() {
        /**
         * A white ambient light source.
         */
        AmbientLight ambient = new AmbientLight();
        ambient.setColor(ColorRGBA.White);
        rootNode.addLight(ambient);
    }

    /**
     * *** There will be water
     * *******************************************************
     */
    private void simpleWater() {
        SimpleWaterProcessor waterProcessor = new SimpleWaterProcessor(assetManager);
        waterProcessor.setReflectionScene(mainScene);

        Vector3f waterLocation = new Vector3f(0, -6, 0);

        waterProcessor.setPlane(new Plane(Vector3f.UNIT_Y, waterLocation.dot(Vector3f.UNIT_Y)));
        viewPort.addProcessor(waterProcessor);

        waterProcessor.setWaterDepth(10);
        waterProcessor.setDistortionScale(0.05f);
        waterProcessor.setWaveSpeed(0.05f);

        Quad quad = new Quad(800, 800);
        quad.scaleTextureCoordinates(new Vector2f(6f, 6f));

        Geometry water = new Geometry("water", quad);
        water.setLocalRotation(new Quaternion().fromAngleAxis(-FastMath.HALF_PI, Vector3f.UNIT_X));
        water.setLocalTranslation(-400, 0.32f, 400);
        water.setShadowMode(RenderQueue.ShadowMode.Receive);
        water.setMaterial(waterProcessor.getMaterial());

        rootNode.attachChild(water);
    }

    /**
     * *** Use this water instead
     * ****************************************************
     */
    void water() {
        FilterPostProcessor fpp = new FilterPostProcessor(assetManager);

        water = new WaterFilter(rootNode, lightDir);
        water.setCenter(Vector3f.ZERO);
        water.setRadius(2600);
        water.setWaveScale(0.003f);
        water.setMaxAmplitude(2.8f);
        //  water.setFoamExistence(new Vector3f(0.0f, 0.0f, 0.0f));
        //  water.setFoamTexture((Texture2D) assetManager.loadTexture("Textures/foam_sea.jpg"));
        water.setRefractionStrength(0.2f);
        water.setWaterHeight(2.7f);

        fpp.addFilter(water);

        viewPort.addProcessor(fpp);
    }

    /**
     * *** There will be a sky
     * *******************************************************
     */
    protected void sky() {
        rootNode.attachChild(SkyFactory.createSky(
                assetManager, "Textures/Sky/Bright/BrightSky.dds", false));
    }

    /**
     * *** Fire
     * **********************************************************************
     */
    private void fire() {
        /**
         * Uses Texture from jme3-test-data library!
         */
        ParticleEmitter fireEffect = new ParticleEmitter("Emitter", ParticleMesh.Type.Triangle, 30);
        Material fireMat = new Material(assetManager, "Common/MatDefs/Misc/Particle.j3md");
        //fireMat.setTexture("Texture", assetManager.loadTexture("Effects/Explosion/flame.png"));
        fireEffect.setMaterial(fireMat);
        fireEffect.setImagesX(2);
        fireEffect.setImagesY(2); // 2x2 texture animation
        fireEffect.setEndColor(new ColorRGBA(1f, 0f, 0f, 1f));   // red
        fireEffect.setStartColor(new ColorRGBA(1f, 1f, 0f, 0.5f)); // yellow
        fireEffect.getParticleInfluencer().setInitialVelocity(new Vector3f(0, 2, 0));
        fireEffect.setStartSize(0.6f);
        fireEffect.setEndSize(0.1f);
        fireEffect.setGravity(0f, 0f, 0f);
        fireEffect.setLowLife(0.5f);
        fireEffect.setHighLife(3f);
        fireEffect.getParticleInfluencer().setVelocityVariation(0.3f);
        rootNode.attachChild(fireEffect);

    }

    /**
     * *** Fog
     * ***********************************************************************
     */
    private void fog() {
        /**
         * Add fog to a scene
         */
        FilterPostProcessor fpp = new FilterPostProcessor(assetManager);
        FogFilter fog = new FogFilter();
        fog.setFogColor(new ColorRGBA(0.9f, 0.9f, 0.9f, 1.0f));
        fog.setFogDistance(155);
        fog.setFogDensity(2.0f);
        fpp.addFilter(fog);
        viewPort.addProcessor(fpp);
    }

    /**
     * *********** Set up the gnomes
     * **************************************************
     */
    private Geometry blackGnome(String name, Vector3f loc) {
        Box box = new Box(1, 1, 1);
        Geometry gnome = new Geometry(name, box);
        gnome.setMaterial(black_mat);
        gnomes.attachChild(gnome);

        gnome.setLocalTranslation(loc);

        box_phy = new RigidBodyControl(2f);

        gnome.addControl(box_phy);
        bulletAppState.getPhysicsSpace().add(box_phy);

        return gnome;
    }

    private Geometry redGnome(String name, Vector3f loc) {
        Sphere box = new Sphere(50, 50, 0.4f);
        Geometry gnome = new Geometry(name, box);
        gnome.setLocalTranslation(loc);
        gnome.setMaterial(red_mat);
        ingredients.attachChild(gnome);
        return gnome;
    }

    /**
     * *** Set the gnomes free
     * *******************************************************
     */
    private void spreadGnomes() {

        // blackGnomes

        gnomes = new Node("Gnomes");
        rootNode.attachChild(gnomes);

        bg1 = blackGnome("Troglodau", new Vector3f(-63.77f, 30f, -63.32f));
        bg2 = blackGnome("Dumbalau", new Vector3f(22, 25, 27));
        bg3 = blackGnome("Prostalau", new Vector3f(88, 33, 22));

        // Red gnomes

        ingredients = new Node("Ingredients");
        rootNode.attachChild(ingredients);

        rg1 = redGnome("Bezoar", new Vector3f(52.72f, 6.61f, -66.3f));
        rg2 = redGnome("Flabberghasted Leech", new Vector3f(175.47f, 4.8f, -24f));
        rg3 = redGnome("Dittany", new Vector3f(-9.99f, 8.67f, 118.03f));
        rg4 = redGnome("Griffin Claw", new Vector3f(77.07f, 8.43f, 104.32f));
        rg5 = redGnome("Vinegar", new Vector3f(78.28f, 6.56f, -123.80f));
        rg6 = redGnome("Doxy Egg", new Vector3f(91.34f, 30f, 77f));

   //     maxPoints = 21;

    }

    private void followPlayer() {
        Vector3f pos = player.getPhysicsLocation();
        // TODO: Enemy to follow player

        System.out.println("You are here: " + pos.x + " " + pos.y + " " + pos.z);
    }

    private void printScore() {
        guiNode.detachChild(score);
        score.setText("Score : " + points + "/" + maxPoints);
        guiNode.attachChild(score);

        System.out.println("printScore");
    }

    /**
     * ***** Set up key control
     * ********************************************************
     */
    private void keys() {
        inputManager.addMapping("Left", new KeyTrigger(KeyInput.KEY_A));
        inputManager.addMapping("Right", new KeyTrigger(KeyInput.KEY_D));
        inputManager.addMapping("Up", new KeyTrigger(KeyInput.KEY_W));
        inputManager.addMapping("Down", new KeyTrigger(KeyInput.KEY_S));
        inputManager.addMapping("Jump", new KeyTrigger(KeyInput.KEY_SPACE));
        inputManager.addMapping("shoot",
                new MouseButtonTrigger(MouseInput.BUTTON_LEFT));
        inputManager.addMapping("pick",
                new MouseButtonTrigger(MouseInput.BUTTON_RIGHT));

        inputManager.addListener(this, "Left");
        inputManager.addListener(this, "Right");
        inputManager.addListener(this, "Up");
        inputManager.addListener(this, "Down");
        inputManager.addListener(this, "Jump");
        inputManager.addListener(this, "shoot");
        inputManager.addListener(this, "pick");
    }

    /**
     * **** Set up the materials
     * ******************************************************
     */
    private void materials() {
        ball_mat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        ball_mat.setColor("Color", ColorRGBA.Blue);

        black_mat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        black_mat.setColor("Color", ColorRGBA.Black);

        red_mat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        red_mat.setColor("Color", ColorRGBA.Red);
    }

    /**
     * *** Collecting aid
     * *************************************************************
     */
    protected void cross() {
        setDisplayStatView(false);
        //guiFont = assetManager.loadFont("Interface/Fonts/Default.fnt");
        ch = new BitmapText(guiFont, false);
        ch.setSize(guiFont.getCharSet().getRenderedSize() * 2);
        ch.setText("o");        // fake crosshairs :)
        ch.setLocalTranslation( // center
                settings.getWidth() / 2,
                settings.getHeight() / 2, 0);
        guiNode.attachChild(ch);

        System.out.println("cross");
    }

    protected void cleanCross() {
        guiNode.detachChild(ch);
        setDisplayStatView(true);
        System.out.println("cleanCross");
    }

    /**
     * *** This is the ball you throw
     * ***********************************************
     */
    public void ball() {
        Geometry ball_geo = new Geometry("ball", ball);
        ball_geo.setMaterial(ball_mat);
        rootNode.attachChild(ball_geo);
        ball_geo.setLocalTranslation(cam.getLocation());
        ball_phy = new RigidBodyControl(1f);
        ball_geo.addControl(ball_phy);
        bulletAppState.getPhysicsSpace().add(ball_phy);
        /**
         * Accelerate the physcial ball to shoot it.
         */
        ball_phy.setLinearVelocity(cam.getDirection().mult(25));
    }

    /**
     * **** Action functions
     * **********************************************************
     */
    public void gameOver() {
        rootNode.detachAllChildren();
        guiNode.detachAllChildren();
        bulletAppState.cleanup();
        stateManager.detach(bulletAppState);
        flyCam.setEnabled(false);


        txt = new BitmapText(guiFont, false);
        txt.setSize(guiFont.getCharSet().getRenderedSize() * 2);
        txt.setText("Congrationations!\n\n" + "You have scared all the gnomes\n\n"
                + "and collected all the ingredients.");
        txt.setLocalTranslation(// center
                settings.getWidth() / 10,
                settings.getHeight() / 1.5f, 0);
        guiNode.attachChild(txt);

        System.out.println("gameOver");

    }

    public void onAction(String name, boolean isPressed, float tpf) {
        if (name.equals("Left")) {
            left = isPressed;
        } else if (name.equals("Right")) {
            right = isPressed;
        } else if (name.equals("Up")) {
            up = isPressed;
        } else if (name.equals("Down")) {
            down = isPressed;
        } else if (name.equals("Jump")) {
            if (isPressed) {
                player.jump();
            }
        } else if (name.equals("shoot")) {
            if (!isPressed) {
                ball();
            }
        } else if (name.equals("pick") && isPressed) {
            cross();
        } else if (name.equals("pick") && !isPressed) {
            // 1. Reset results list.
            cleanCross();
            CollisionResults results = new CollisionResults();
            // 2. Aim the ray from cam loc to cam direction.
            Ray ray = new Ray(cam.getLocation(), cam.getDirection());
            // 3. Collect intersections between Ray and Shootables in results list.
            ingredients.collideWith(ray, results);
            // 4. Print the results
            for (int i = 0; i < results.size(); i++) {
                // For each hit, we know distance, impact point, name of geometry.
                float dist = results.getCollision(i).getDistance();
                Vector3f pt = results.getCollision(i).getContactPoint();
                String hit = results.getCollision(i).getGeometry().getName();;
                fpsText.setText("Collected " + hit + "!");
                ingredients.detachChildNamed(hit);
                points++;

            }
        }
        if (points >= maxPoints) {
            gameOver();
            return;
        }
        followPlayer();
    }

    @Override
    public void simpleUpdate(float tpf) {
        System.out.println("simpleUpdate");
        camDir.set(cam.getDirection()).multLocal(0.6f);
        camLeft.set(cam.getLeft()).multLocal(0.4f);
        walkDir.set(0, 0, 0);
        if (left) {
            walkDir.addLocal(camLeft);
        }
        if (right) {
            walkDir.addLocal(camLeft.negate());
        }
        if (up) {
            walkDir.addLocal(camDir);
        }
        if (down) {
            walkDir.addLocal(camDir.negate());
        }
        player.setWalkDirection(walkDir);
        enemy.setWalkDirection(walkDir.negate());
        //    followPlayer();
        cam.setLocation(player.getPhysicsLocation());

        if (points >= maxPoints) {
            gameOver();
        } else {
            printScore();
        }
    }

    @Override
    public void simpleRender(RenderManager rm) {
        //TODO: add render code
    }

    public void collision(PhysicsCollisionEvent event) {
        System.out.println("collision!");
        if (event.getNodeA() == null || event.getNodeB() == null) {
            return;
        }

        //   System.out.println(event.getNodeA().getName() + " by " + event.getNodeB().getName());

        if (("Troglodau").equals(event.getNodeA().getName()) && ("ball").equals(event.getNodeB().getName())) {
            fpsText.setText("You hit Troglodau!");
            hitsTroglodau++;
            if (hitsTroglodau == 3) {
                hitsTroglodau = 0;
                points += 3;
                gnomes.detachChildNamed("Troglodau");
                printScore();
            }
        }

        if (("Dumbalau").equals(event.getNodeA().getName()) & ("ball").equals(event.getNodeB().getName())) {
            fpsText.setText("You hit Dumbalau!");
            hitsDumbalau++;
            if (hitsDumbalau == 3) {
                hitsDumbalau = 0;
                points += 3;
                gnomes.detachChildNamed("Dumbalau");
                printScore();
            }
        }

        if (("Prostalau").equals(event.getNodeA().getName()) & ("ball").equals(event.getNodeB().getName())) {
            fpsText.setText("You hit Prostalau!");
            hitsProstalau++;
            if (hitsProstalau == 3) {
                hitsProstalau = 0;
                points += 3;
                gnomes.detachChildNamed("Prostalau");
                printScore();
            }
        }

        if (points >= maxPoints) {
            // gameOver();
            return;
        }
    }

    /**
     * ***************** OTHER *******************
     */
    private PhysicsSpace getPhysicsSpace() {
        return bulletAppState.getPhysicsSpace();
    }
}
