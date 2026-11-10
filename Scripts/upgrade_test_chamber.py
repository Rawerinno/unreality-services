"""
UNREALITY SERVICES / Test Chamber 01 / Art pass v1
Run from Unreal Editor's Output Log:
    py "C:/Dev/unreality-services/Scripts/upgrade_test_chamber.py"

This script ENHANCES the open L_TestChamber_01, does not replace the map.
Re-running updates/reuses labeled actors and material assets.
UE 5.8 + Python Editor Script Plugin. Safe to retain the existing level.
"""
import unreal

LEVEL_NAME = "L_TestChamber_01"
CONTENT = "/Game/UnrealityServices"
MATS = CONTENT + "/Materials"

levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
world = unreal.EditorLevelLibrary.get_editor_world()
if LEVEL_NAME not in world.get_path_name():
    raise RuntimeError("Open L_TestChamber_01 in the editor before running the art pass. Current world: " + world.get_path_name())

def log(message):
    unreal.log("[UNREALITY SERVICES | CHAMBER] " + str(message))

def warning(message):
    unreal.log_warning("[UNREALITY SERVICES | CHAMBER] " + str(message))

asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
cube = unreal.load_asset("/Engine/BasicShapes/Cube")
if not cube:
    raise RuntimeError("Engine BasicShapes/Cube mesh unavailable")

palette = {
    "M_Concrete": (0.105, 0.143, 0.160, 0.82, 0.00, None),
    "M_Wall":     (0.058, 0.101, 0.125, 0.52, 0.23, None),
    "M_Steel":    (0.067, 0.104, 0.121, 0.34, 0.80, None),
    "M_Pale":     (0.41, 0.55, 0.57, 0.48, 0.10, None),
    "M_Orange":   (0.94, 0.37, 0.085, 0.42, 0.18, None),
    "M_Cyan":     (0.055, 0.84, 0.92, 0.31, 0.20, None),
    "M_Emissive": (0.04, 0.15, 0.17, 0.35, 0.02, (0.20, 2.6, 3.1)),
    "M_AmberGlow": (0.25, 0.10, 0.01, 0.40, 0.02, (3.2, 1.0, 0.05)),
}

def material(name, spec):
    path = MATS + "/" + name
    existing = unreal.load_asset(path)
    if existing:
        return existing
    base, green, blue, rough, metallic, emissive = spec
    mat = asset_tools.create_asset(name, MATS, unreal.Material, unreal.MaterialFactoryNew())
    if not mat:
        raise RuntimeError("Could not create material " + path)

    ml = unreal.MaterialEditingLibrary
    def rgb(values, x, y, prop):
        node = ml.create_material_expression(mat, unreal.MaterialExpressionConstant3Vector, x, y)
        node.set_editor_property("constant", unreal.LinearColor(values[0], values[1], values[2], 1.0))
        ml.connect_material_property(node, "", prop)

    rgb((base, green, blue), -450, 0, unreal.MaterialProperty.MP_BASE_COLOR)
    node = ml.create_material_expression(mat, unreal.MaterialExpressionConstant, -450, 250)
    node.set_editor_property("r", rough)
    ml.connect_material_property(node, "", unreal.MaterialProperty.MP_ROUGHNESS)
    node = ml.create_material_expression(mat, unreal.MaterialExpressionConstant, -450, 470)
    node.set_editor_property("r", metallic)
    ml.connect_material_property(node, "", unreal.MaterialProperty.MP_METALLIC)
    if emissive:
        rgb(emissive, -450, 720, unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    errors = ml.recompile_material(mat)
    if errors:
        warning("Material warnings for " + name + ": " + str(errors))
    unreal.EditorAssetLibrary.save_loaded_asset(mat)
    return mat

materials = {}
for name, values in palette.items():
    materials[name] = material(name, values)

def lookup(label):
    for actor in actors.get_all_level_actors():
        if actor.get_actor_label() == label:
            return actor
    return None

def geometry(label, pos, scale, mat, collision=True):
    actor = lookup(label)
    if actor is None:
        actor = actors.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(*pos))
        if not actor:
            raise RuntimeError("Failed to spawn " + label)
        actor.set_actor_label(label)
    actor.set_actor_location(unreal.Vector(*pos), False, False)
    actor.set_actor_scale3d(unreal.Vector(*scale))
    mesh = actor.static_mesh_component
    mesh.set_static_mesh(cube)
    mesh.set_material(0, materials[mat])
    if not collision:
        mesh.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
    return actor

# Repaint existing architecture first; no overlap or deletion.
for suffix, mat in [
    ("Floor_Concrete", "M_Concrete"),
    ("Wall_North", "M_Wall"), ("Wall_South", "M_Wall"),
    ("Wall_East", "M_Wall"), ("Wall_West", "M_Wall"),
    ("IntakePad", "M_Cyan"), ("OutputPad", "M_Orange"),
    ("CentralDivider", "M_Emissive"),
]:
    a = lookup("US_" + suffix)
    if a and isinstance(a, unreal.StaticMeshActor):
        a.static_mesh_component.set_material(0, materials[mat])
for a in actors.get_all_level_actors():
    label = a.get_actor_label()
    if isinstance(a, unreal.StaticMeshActor):
        if label.startswith("US_CeilingRail_") or label.startswith("US_OverheadRib_"):
            a.static_mesh_component.set_material(0, materials["M_Steel"])
        if label.startswith("US_MeasurementLane_"):
            a.static_mesh_component.set_material(0, materials["M_Pale"])

# Suspended luminous ceiling fixtures; non-shadow-casting complement to the light actors.
for x in (-850, 0, 850):
    for y in (-550, 0, 550):
        geometry("US_CeilingLightPanel_%s_%s" % (x, y),
                 (x, y, 603), (2.4, 0.52, 0.075), "M_Emissive", False)
        geometry("US_CeilingLightTrim_%s_%s" % (x, y),
                 (x, y, 611), (2.65, 0.69, 0.075), "M_Steel", False)

# Floor markers, machinery ribs and clean sci-fi municipal signage bands.
for y, mat in [(-290, "M_Cyan"), (290, "M_Orange")]:
    geometry("US_PadGuide_%s" % y, (-250, y, 19.0),
             (3.1, 0.12, 0.025), mat, False)
    geometry("US_PadLeft_%s" % y, (-401, y, 15),
             (0.07, 3.0, 0.025), mat, False)
    geometry("US_PadRight_%s" % y, (-99, y, 15),
             (0.07, 3.0, 0.025), mat, False)
for x in (-1000, -450, 100, 650, 1100):
    for y in (-935, 935):
        geometry("US_Pillar_%s_%s" % (x, y), (x, y, 260),
                 (0.42, 0.40, 5.2), "M_Steel")
        geometry("US_PillarLumen_%s_%s" % (x, y),
                 (x, y - (9 if y > 0 else -9), 350),
                 (0.09, 0.055, 2.0), "M_Emissive", False)
for x in (-1000, -550, 0, 550, 1000):
    geometry("US_Centerline_%s" % x, (x, 0, 2.5),
             (0.55, 0.10, 0.04), "M_Emissive", False)
# Upper wall accent bands
for y in (-947, 947):
    geometry("US_WallBand_%s" % y, (0, y, 420),
             (25.8, 0.13, 0.15), "M_Orange", False)
for x in (-1297, 1297):
    geometry("US_WallEdge_%s" % x, (x, 0, 420),
             (0.12, 18.7, 0.15), "M_Cyan", False)

# Existing fixed lights were too dim for an enclosed room without a skylight.
# Provide strong, inexpensive fill light from several non-shadow-casting point lamps.
for i, (x, y) in enumerate([
    (-850, -570), (-850, 570), (0, -570), (0, 570),
    (850, -570), (850, 570), (-250, 0),
]):
    label = "US_ArtFill_%02d" % i
    a = lookup(label)
    if not a:
        a = actors.spawn_actor_from_class(unreal.PointLight, unreal.Vector(x, y, 470))
        a.set_actor_label(label)
    comp = a.point_light_component
    comp.set_editor_property("intensity", 40000.0)
    comp.set_editor_property("attenuation_radius", 1450.0)
    comp.set_editor_property("cast_shadows", False)

# Existing lighting is kept, but boost its reach.
for a in actors.get_all_level_actors():
    if a.get_actor_label().startswith("US_WorkLamp_") and isinstance(a, unreal.PointLight):
        c = a.point_light_component
        c.set_editor_property("intensity", 32000.0)
        c.set_editor_property("attenuation_radius", 1050.0)
        c.set_editor_property("cast_shadows", False)

# A straightforward overhead player-readable light, with color temperatures and readable exposure.
a = lookup("US_MasterFill")
if a is None:
    a = actors.spawn_actor_from_class(unreal.DirectionalLight,
        unreal.Vector(0, 0, 600), unreal.Rotator(-67, -35, 0))
    a.set_actor_label("US_MasterFill")
a.directional_light_component.set_editor_property("intensity", 3.5)
a.directional_light_component.set_editor_property("cast_shadows", False)

# Give the two test bodies a recognizable identity without touching physical properties.
for name, mat in (("BODY_A", "M_Cyan"), ("BODY_B", "M_Orange")):
    a = lookup(name)
    if a and hasattr(a, "mesh"):
        a.mesh.set_material(0, materials[mat])
    else:
        warning(name + " not found (this is fine if your C++ classes are not loaded yet).")

# Save the same level, never create a replacement .umap.
if not levels.save_current_level():
    raise RuntimeError("Level could not be saved. Read the Output Log.")
log("Art pass complete. Updated existing Test Chamber 01 (lights, materials, trim).")
log("Use Lit viewport mode. Press Play after checking the scene is illuminated.")
