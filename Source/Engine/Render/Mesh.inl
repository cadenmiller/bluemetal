
template<typename TVertex>
blMesh(std::shared_ptr<blDevice> device, const std::vector<TVertex>& vertices, const std::vector<uint32_t>& indices)
    : _device(device)
{
    setVertices(vertices);
    setIndices(indices);
}
    

template<typename TVertex>
void setVertices(const std::vector<TVertex>& vertices)
{
    // Create the vertex and index buffers
    size_t vbSize = sizeof(blVertex) * vertices.size();
    
    VmaAllocationInfo allocInfo = {};
    blBuffer stagingBuffer(_device, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vbSize, &allocInfo, true);

    memcpy(allocInfo.pMappedData, vertices.data(), vbSize);

    _vertexBuffer = std::make_unique<blBuffer>(_device, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vbSize);

    _device->immediateSubmit(
        [&](VkCommandBuffer cmd)
        {
            VkBufferCopy region = { 0, 0, vbSize };
            vkCmdCopyBuffer(cmd, stagingBuffer.getBuffer(), _vertexBuffer->getBuffer(), 1, &region);
        });
   
}