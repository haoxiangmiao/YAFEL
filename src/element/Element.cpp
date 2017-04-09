//
// Created by tyler on 3/14/17.
//

#include "element/Element.hpp"
#include "element/ShapeFunctionUtils.hpp"
#include "utils/DoFManager.hpp"

YAFEL_NAMESPACE_OPEN

Element::Element(ElementType et, int dofpn)
        : elementType(et),
          localMesh(Mesh::DefinitionScheme::Explicit),
          dof_per_node(dofpn)
{

    switch (et.elementTopology) {
        case ElementTopology::TensorProduct:
            make_tensorProduct();
            break;
        case ElementTopology::Simplex:
            make_simplex();
            break;
        case ElementTopology::None:
            //null element. used for points/unsupported types
            break;
    }
}


void Element::make_tensorProduct()
{

    //auto lob_pts_1d = make_LobattoPoints_1D(elementType.polyOrder);
    auto qr = QuadratureRule(elementType.polyOrder + 1,
                             QuadratureRule::QuadratureType::GAUSS_LOBATTO);
    std::vector<double> lob_pts_1d;
    for (auto x : qr.nodes)
        lob_pts_1d.push_back(x(0));

    auto npts = static_cast<int>(lob_pts_1d.size());

    std::vector<coordinate<>> localPoints_xi;
    std::vector<int> cellNodes;
    std::vector<int> offsets;
    std::vector<CellType> cellTypes;

    if (elementType.topoDim == 1) {
        //make a line element
        localPoints_xi.resize(npts);
        cellNodes.resize(2 * (npts - 1));
        offsets.resize(npts);
        cellTypes.resize(npts - 1, CellType::Line2);


        for (int i = 0; i < npts; ++i) {
            localPoints_xi[i](0) = lob_pts_1d[i];
        }

        int offset = 0;
        for (int i = 0; i < npts - 1; ++i) {
            offsets[i] = offset;
            offset += 2;

            cellNodes[2 * i] = i;
            cellNodes[2 * i + 1] = i + 1;
        }
        offsets[npts - 1] = offset;

    } else if (elementType.topoDim == 2) {
        // make a quad element

        localPoints_xi.resize(npts * npts);
        cellNodes.resize(4 * (npts - 1) * (npts - 1));
        offsets.resize((npts - 1) * (npts - 1) + 1);
        cellTypes.resize((npts - 1) * (npts - 1), CellType::Quad4);

        int idx = 0;
        for (int i = 0; i < npts; ++i) {
            for (int j = 0; j < npts; ++j) {
                localPoints_xi[idx](1) = lob_pts_1d[i];
                localPoints_xi[idx](0) = lob_pts_1d[j];
                ++idx;
            }
        }

        int offset = 0;
        idx = 0;
        for (int i = 0; i < npts - 1; ++i) {
            for (int j = 0; j < npts - 1; ++j) {

                cellNodes[offset + 0] = i * npts + j;
                cellNodes[offset + 1] = i * npts + j + 1;
                cellNodes[offset + 2] = (i + 1) * npts + j + 1;
                cellNodes[offset + 3] = (i + 1) * npts + j;

                offsets[idx++] = offset;
                offset += 4;
            }
        }
        offsets[idx] = offset;


    } else if (elementType.topoDim == 3) {
        // make a hex element

        localPoints_xi.resize(npts * npts * npts);
        cellNodes.resize(8 * (npts - 1) * (npts - 1) * (npts - 1));
        offsets.resize((npts - 1) * (npts - 1) * (npts - 1) + 1);
        cellTypes.resize((npts - 1) * (npts - 1) * (npts - 1), CellType::Quad4);

        int idx = 0;
        for (int i = 0; i < npts; ++i) {
            for (int j = 0; j < npts; ++j) {
                for (int k = 0; k < npts; ++k) {
                    localPoints_xi[idx](2) = lob_pts_1d[i];
                    localPoints_xi[idx](1) = lob_pts_1d[j];
                    localPoints_xi[idx](0) = lob_pts_1d[k];
                    ++idx;
                }
            }
        }

        int offset = 0;
        idx = 0;
        for (int i = 0; i < npts - 1; ++i) {
            for (int j = 0; j < npts - 1; ++j) {
                for (int k = 0; k < npts - 1; ++k) {

                    cellNodes[offset + 0] = (i * npts + j) * npts + k;
                    cellNodes[offset + 1] = (i * npts + j) * npts + k + 1;
                    cellNodes[offset + 2] = (i * npts + j + 1) * npts + k + 1;
                    cellNodes[offset + 3] = (i * npts + j + 1) * npts + k;
                    cellNodes[offset + 4] = ((i + 1) * npts + j) * npts + k;
                    cellNodes[offset + 5] = ((i + 1) * npts + j) * npts + k + 1;
                    cellNodes[offset + 6] = ((i + 1) * npts + j + 1) * npts + k + 1;
                    cellNodes[offset + 7] = ((i + 1) * npts + j + 1) * npts + k;

                    offsets[idx++] = offset;
                    offset += 8;


                }
            }
        }
        offsets[idx] = offset;

    } else {
        // unsupported topoDim
    }

    // Set local mesh members from created containers
    localMesh.setGeometryNodes(std::move(localPoints_xi));
    localMesh.setCellNodes(std::move(cellNodes));
    localMesh.setOffsets(std::move(offsets));
    localMesh.setCellTypes(std::move(cellTypes));

    quadratureRule = QuadratureRule::make_tensor_product(QuadratureRule::QuadratureType::GAUSS_LEGENDRE,
                                                         elementType.topoDim, 2*elementType.polyOrder);

    tensor_product_shape_functions(localMesh.getGeometryNodes(),
                                   quadratureRule.nodes,
                                   elementType.topoDim,
                                   shapeValues,
                                   shapeGradXi);

}


void Element::make_simplex()
{
    auto qr = QuadratureRule(elementType.polyOrder + 1,
                             QuadratureRule::QuadratureType::GAUSS_LOBATTO);
    std::vector<double> lob_pts_1d;
    for (auto x : qr.nodes)
        lob_pts_1d.push_back(x(0));
    auto npts = static_cast<int>(lob_pts_1d.size());

    std::vector<coordinate<>> localPoints_xi;
    std::vector<int> cellNodes;
    std::vector<int> offsets;
    std::vector<CellType> cellTypes;


    if (elementType.topoDim == 1) {
        //1D simplex is a line, which is also a 1D tensor-product element
        make_tensorProduct();
        return;
    } else if (elementType.topoDim == 2) {
        // based on scheme be Blyth & Pozrikdis (JAM 2005)
        localPoints_xi.resize((npts * (npts + 1)) / 2);
        int nt_local = elementType.polyOrder * elementType.polyOrder;
        cellNodes.resize(3 * nt_local);
        offsets.reserve(nt_local + 1);
        cellTypes.resize(nt_local, CellType::Tri3);

        int idx = 0;
        for (int i = 0; i < npts; ++i) {
            for (int j = 0; j < npts - i; ++j) {
                int k = npts - i - j - 1;
                double vi = (lob_pts_1d[i] + 1) / 2;
                double vj = (lob_pts_1d[j] + 1) / 2;
                double vk = (lob_pts_1d[k] + 1) / 2;
                localPoints_xi[idx](0) = (1 + 2 * vj - vi - vk) / 3;
                localPoints_xi[idx](1) = (1 + 2 * vi - vj - vk) / 3;
                ++idx;
            }
        }

        //make local triangles
        idx = 0;
        int offset = 0;
        for (int layer = 0; layer < elementType.polyOrder; ++layer) {
            for (int i = 0; i < elementType.polyOrder - layer; ++i) {

                cellNodes[offset + 0] = idx + i;
                cellNodes[offset + 1] = idx + i + 1;
                cellNodes[offset + 2] = idx + i + 1 + elementType.polyOrder - layer;
                offsets.push_back(offset);
                offset += 3;
                if (i < elementType.polyOrder - layer - 1) {
                    cellNodes[offset + 0] = idx + i + 1;
                    cellNodes[offset + 1] = idx + i + 2 + elementType.polyOrder - layer;
                    cellNodes[offset + 2] = idx + i + 1 + elementType.polyOrder - layer;
                    offsets.push_back(offset);
                    offset += 3;
                }

            }
            idx += elementType.polyOrder + 1 - layer;
        }
        offsets.push_back(offset);


    } else if (elementType.topoDim == 3) {

    } else {
        //unsupported topoDim
    }

    localMesh.setGeometryNodes(std::move(localPoints_xi));
    localMesh.setCellNodes(std::move(cellNodes));
    localMesh.setOffsets(std::move(offsets));
    localMesh.setCellTypes(std::move(cellTypes));
}


YAFEL_NAMESPACE_CLOSE