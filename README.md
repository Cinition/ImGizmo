<div align="center">
    <img src="./logo.svg" alt="" width="192" align="center" />
    <h1 align="center">ImGizmo</h1>
</div>

ImGizmo is a Dear ImGui extension that focusing on giving the library the functionality to draw gizmos that are needed for scene manipulation, for both 3D and 2D. ImGizmo strives for great UX and bases most of its functionality and usability on the great work by [Robin-Yann Storm](https://rystorm.com/blog/translate-gizmo-design).

---

## Usage
To start using ImGizmo inside of your project you just have to include ``ImGizmo.h`` inside of your project after including ``imgui.h``.

Then same as ImGui you have to create the global context and at the end destroy it by calling.
```c++
// For creation
ImGizmo::CreateContext();

// ... Your entire codebase in between

// For destroying
ImGizmo::DestroyContext();
```

### ImGizmoSpace

ImGizmo works on the principle of 'Spaces', these are

You can start and end a ImGizmo 'Space' by calling
```c++
// For beginning a space
ImGizmo::Begin();

// And for ending the current space
ImGizmo::End();

// Usual code
if (ImGizmo::Begin("Cool Space", YOUR_VIEW_MATRIX, YOUR_TRANSFORM_MATRIX))
{
    //...Gizmo drawing
    ImGizmo::End();
}
```

### Gizmos

ImGizmo currently can draw the following gizmos:

- ``GizmoTranslate``

- ``GizmoRotateEuler`` / ``GizmoRotateQuat``

- ``GizmoScale``

- ``GizmoPoint``

### Styling

***~WIP~***

*Planned to be able to choose your own colors and sizes for all gizmos*

---

## Examples

ImGizmo provides example of how this library could be used, and provides this inside of the "Examples" directory, to build the examples project/code ImGizmo uses xmake..

### Building and Running the examples
You can now just run ``xmake`` in the base directory of the repository to install all dependencies for the examples, now just build and run the examples project.

```
// build the project
xmake build

// run the project
xmake run
```

## License

ImGizmo, just like Dear ImGui makes use of the MIT license see LICENSE.txt for more information