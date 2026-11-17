import SwiftUI
import MetalKit

struct EmulatorScreenView: UIViewRepresentable {
    let bridge: PS2Bridge

    func makeCoordinator() -> Renderer {
        Renderer(bridge: bridge)
    }

    func makeUIView(context: Context) -> MTKView {
        let mtkView = MTKView()
        mtkView.device = MTLCreateSystemDefaultDevice()
        mtkView.delegate = context.coordinator
        mtkView.framebufferOnly = false
        mtkView.preferredFramesPerSecond = 60
        return mtkView
    }

    func updateUIView(_ uiView: MTKView, context: Context) {}

    class Renderer: NSObject, MTKViewDelegate {
        let bridge: PS2Bridge
        let device: MTLDevice
        let commandQueue: MTLCommandQueue
        var texture: MTLTexture?

        init(bridge: PS2Bridge) {
            self.bridge = bridge
            self.device = MTLCreateSystemDefaultDevice()!
            self.commandQueue = device.makeCommandQueue()!
            super.init()

            let desc = MTLTextureDescriptor.texture2DDescriptor(
                pixelFormat: .rgba8Unorm,
                width: Int(bridge.framebufferWidth()),
                height: Int(bridge.framebufferHeight()),
                mipmapped: false
            )
            desc.usage = [.shaderRead]
            self.texture = device.makeTexture(descriptor: desc)
        }

        func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {}

        func draw(in view: MTKView) {
            bridge.runFrame()

            guard let texture = texture,
                  let ptr = bridge.framebufferPointer(),
                  let drawable = view.currentDrawable,
                  let commandBuffer = commandQueue.makeCommandBuffer(),
                  let blitEncoder = commandBuffer.makeBlitCommandEncoder() else { return }

            let width = Int(bridge.framebufferWidth())
            let height = Int(bridge.framebufferHeight())
            let region = MTLRegionMake2D(0, 0, width, height)

            texture.replace(region: region, mipmapLevel: 0, withBytes: ptr, bytesPerRow: width * 4)

            let copyW = min(width, drawable.texture.width)
            let copyH = min(height, drawable.texture.height)
            blitEncoder.copy(
                from: texture, sourceSlice: 0, sourceLevel: 0,
                sourceOrigin: MTLOrigin(x: 0, y: 0, z: 0),
                sourceSize: MTLSize(width: copyW, height: copyH, depth: 1),
                to: drawable.texture, destinationSlice: 0, destinationLevel: 0,
                destinationOrigin: MTLOrigin(x: 0, y: 0, z: 0)
            )
            blitEncoder.endEncoding()

            commandBuffer.present(drawable)
            commandBuffer.commit()
        }
    }
}
