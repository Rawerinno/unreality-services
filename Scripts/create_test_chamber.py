"""
UNREALITY SERVICES | Test Chamber 01
Run from Unreal Editor: Tools > Execute Python Script.
Creates /Game/UnrealityServices/Maps/L_TestChamber_01 as an actual .umap.

Requires Unreal Engine 5.8 with Python Editor Script Plugin enabled.
This script never overwrites an existing test chamber.
"""
import unreal

MAP = "/Game/UnrealityServices/Maps/L_TestChamber_01"
PREFIX = "US_"

def log(msg):
    unreal.log("[UNREALITY SERVICES] " + str(msg))

if unreal.EditorAssetLibrary.does_asset_exist(MAP):
    raise RuntimeError("Test Chamber already exists. No files overwritten: " + MAP)

levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

if not levels.new_level(MAP):
    raise RuntimeError("Could not create " + MAP)

cube = unreal.load_asset("/Engine/BasicShapes/Cube")
if not cube:
    raise RuntimeError("Engine cube mesh not found")

def static(name, pos, scale, collision=True):
    actor = actors.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(*pos))
    actor.set_actor_label(PREFIX + name)
    component = actor.static_mesh_component
    component.set_static_mesh(cube)
    actor.set_actor_scale3d(unreal.Vector(*scale))
    if not collision:
        component.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
    return actor

# Dimensions are centimeters; a UE cube is 100 x 100 x 100 cm.
# Main floor top = Z 0. Walls form a closed test bay with room to push objects.
static("Floor_Concrete", (0, 0, -50), (26, 19, 1))
static("Wall_North", (0, 975, 330), (26, 0.5, 6.6))
static("Wall_South", (0, -975, 330), (26, 0.5, 6.6))
static("Wall_East", (1325, 0, 330), (0.5, 19, 6.6))
static("Wall_West", (-1325, 0, 330), (0.5, 19, 6.6))

# Large structural beams and elevated lighting recesses.
for y in (-810, 810):
    static("CeilingRail_" + str(y), (0, y, 600), (25, 0.18, 0.22))
for x in (-1050, 0, 1050):
    static("OverheadRib_" + str(x), (x, 0, 620), (0.22, 18, 0.22))

# Delineated maintenance platforms and a transfer lane.
static("IntakePad", (-250, -290, 9), (2.9, 2.9, 0.18))
static("OutputPad", (-250, 290, 9), (2.9, 2.9, 0.18))
static("CentralDivider", (-250, 0, 7), (6.8, 0.12, 0.14), False)
for idx in range(5):
    x = 100 + idx * 185
    static("MeasurementLane_" + str(idx), (x, 0, 2), (0.12, 8.0, 0.04), False)

# Spawn gameplay mass blocks from C++ classes, not inert decorative cubes.
block_class = unreal.load_class(None, "/Script/UNREALITY_SERVICES.RealityMassBlock")
if block_class is None:
    raise RuntimeError("C++ RealityMassBlock class unavailable. Compile the C++ modules before generating the map.")
for name, y, kg in (("BODY_A", -290, 20.0), ("BODY_B", 290, 5.0)):
    block = actors.spawn_actor_from_class(block_class, unreal.Vector(-250, y, 180))
    block.set_actor_label(name)
    block.set_editor_property("initial_mass_kg", kg)

spawn = actors.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(-990, 0, 110),
                                      unreal.Rotator(0, 0, 0))
spawn.set_actor_label(PREFIX + "TechnicianSpawn")

# Illuminated industrial atmosphere without expensive post-process or ray tracing.
sun = actors.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 550),
                                     unreal.Rotator(-55, -35, 0))
sun.set_actor_label(PREFIX + "KeyLight")

for i, (x, y) in enumerate(((-650, -570), (-650, 570), (620, -570), (620, 570))):
    light = actors.spawn_actor_from_class(unreal.PointLight, unreal.Vector(x, y, 480))
    light.set_actor_label(PREFIX + "WorkLamp_" + str(i))
    comp = light.point_light_component
    comp.set_editor_property("intensity", 2800.0)
    comp.set_editor_property("attenuation_radius", 760.0)

# Blueprint-derived default player game mode remains defined in Config/DefaultEngine.ini.
if not levels.save_current_level():
    raise RuntimeError("Could not save chamber. Check the Output Log.")
log("Created and saved " + MAP)
log("Press Play: look for BODY_A and BODY_B in the marked lanes.")
