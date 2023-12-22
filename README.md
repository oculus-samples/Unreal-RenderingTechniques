# Rendering Techniques

To change which map is launched, adjust the default map in the project settings, or use the Launch button directly within the editor rather than using the Project Launcher.

### Cascaded Shadows

This map demonstrates how you can have dynamic shadows, including on dynamic objects, using Unreal Engine's cascaded shadow map functionality.

The sliders in the Cascaded Shadows map can be dragged using the triggers on the controller. Some settings are not exposed in the in-game GUI and need to be adjusted directly on the light. The light's rotation is controlled by a Blueprint script attached to the light.

Lights must currently be movable for cascaded shadows to work; stationary lights will display correctly in the editor but will not be shadowed on the device. Note also that shadows may disappear with certain camera/light direction combinations if the far distance is set too far on the light.

### Color Grading

This map demonstrates how to use look-up tables (LUTs) to adjust the look of your scene.

Color grading requires that the patch in the "ColorGradingLUT UE4 Patch" folder is applied to the UE4 source code. With the patch applied, color grading LUTs can be applied as described in the [UE4 support document on LUTs](https://docs.unrealengine.com/en-US/Engine/Rendering/PostProcessEffects/UsingLUTs/index.html).

Note that by default the patch computes the LUT only once rather than rebuilding it every frame. If you have changing or animated postprocessing conditions, this can be changed by adjusting:

```
if (!Views[0].GetTonemappingLUT())
{
    // Rather than regenerating the LUT every frame on mobile, we assume that it is constant to save GPU processing.
    FRDGBuilder GraphBuilder(RHICmdList);
    AddCombineLUTPass(GraphBuilder, Views[0]);
    GraphBuilder.Execute();
}
```

to remove the `if` statement; that is:

```
FRDGBuilder GraphBuilder(RHICmdList);
AddCombineLUTPass(GraphBuilder, Views[0]);
GraphBuilder.Execute();
```

### Distance Field Baked Shadows

Signed Distance Field shadows are an Unreal Engine feature for hard-edged, high-quality precomputed shadows for stationary or static lights and objects.

For precomputed distance field shadows to work, the light type must be set to stationary and "Support Distance Field Shadows" must be enabled within the Project Settings' Mobile Shader Permutation Reduction section.

### Portals

PortalsMap demonstrates two different ways of rendering portals in VR.

The first is using a static parallax-corrected cubemap captured at the location of the portal being looked through. The PortalSampleCubemapMaterial applied to that portal applies parallax correction using the DistanceLimitedReflections node; this makes it appear as if all of the scene is on the surface of a sphere when viewed through the portal. The benefits of this technique are that it can be prebaked and is therefore fairly inexpensive and can be high resolution; however, it also has an obvious warping effect, and objects lack the correct depth when viewed in stereo.

The second method uses stereo render targets to render the scene from the perspective of what each eye would see through the portal. This achieves a very convincing effect, albeit at a higher performance cost. The implementation is contained within the Portals plugin, and the functionality is split between the PortalCaptureActorBP blueprint, the MF_ScreenAlignedUV_VR material function, and the PortalSampleMaterial material. There are also some helper C++ methods contained in the PortalCaptureActor class.

### Text Rendering

The TextRendering map demonstrates the effects of different texture filtering settings in combination with high-contrast textures like text, and also shows how VR stereo layers can be used to render high-quality text or textures.

Note that when rendering text rendering with transparency the text should not be premultiplied with the alpha channel.

## How to Use

### Load the project

First, ensure you have Git LFS installed by running this command:
```sh
git lfs install
```

Then, clone this repo using the "Code" button above, or this command:
```sh
git clone https://github.com/oculus-samples/Unreal-RenderingTechniques
```

### Launch the project in the Unreal Editor using one of the following options.

#### Epic Games Launcher with MetaXR plugin

The easiest way to get started is to use the prebuilt Unreal Engine from the Epic Games Launcher, with MetaXR plugin.

1. Install the [Epic Games Launcher](https://www.epicgames.com/store/en-US/download)
2. In the launcher, install UE5 (recommended).
3. Download and install the MetaXR plugin from the [Unreal Engine 5 Integration download page](https://developer.oculus.com/downloads/package/unreal-engine-5-integration).
3. Launch the Unreal Editor
4. From "Recent Projects", click "Browse" and select `RenderingTechniques.uproject`

#### Meta fork of Epic’s Unreal Engine

The Meta fork of Epic’s Unreal Engine will give you the most up to date integration of Oculus features. However, you must build the editor from its source.

Follow the instructions on [Accessing Unreal Engine source code on GitHub](https://www.unrealengine.com/en-US/ue-on-github) to obtain:
- an Epic account
- a GitHub account
- authorization to access the Unreal Engine source repository
Disregard instructions on downloading Epic’s Unreal Engine source code as you will be building the Meta fork of Epic’s Unreal Engine source.

Make sure you have Visual Studio installed properly:
- Launch the Visual Studio Installer and click Modify for the Visual Studio version you want to use.
- Under the Workloads tab, click Game development with C++ if it isn’t checked and then click Modify.

1. Download the source code from the [Meta fork of Epic’s Unreal Engine on GitHub](https://github.com/Oculus-VR/UnrealEngine).
2. Follow Epic’s instructions on [Building Unreal Engine from Source](https://docs.unrealengine.com/5.2/en-US/building-unreal-engine-from-source/) to complete the process.

Depending on your machine, the build may take awhile to complete.

# Licenses
The Meta License applies to the SDK and supporting material. The MIT License applies to only certain, clearly marked documents. If an individual file does not indicate which license it is subject to, then the Meta License applies.
