CREATE DATABASE IF NOT EXISTS db_supermercado
CHARACTER SET utf8mb4
COLLATE utf8mb4_unicode_ci;

USE db_supermercado;

SET FOREIGN_KEY_CHECKS = 0;
DROP TABLE IF EXISTS compras_detalle;
DROP TABLE IF EXISTS compras;
DROP TABLE IF EXISTS ventas_detalle;
DROP TABLE IF EXISTS ventas;
DROP TABLE IF EXISTS productos;
DROP TABLE IF EXISTS empleados;
DROP TABLE IF EXISTS proveedores;
DROP TABLE IF EXISTS clientes;
DROP TABLE IF EXISTS puestos;
DROP TABLE IF EXISTS marcas;
SET FOREIGN_KEY_CHECKS = 1;

CREATE TABLE marcas (
    id_marca SMALLINT NOT NULL AUTO_INCREMENT,
    marca VARCHAR(50) NOT NULL,
    PRIMARY KEY (id_marca),
    UNIQUE KEY uk_marcas_marca (marca)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE puestos (
    id_puesto SMALLINT NOT NULL AUTO_INCREMENT,
    puesto VARCHAR(50) NOT NULL,
    PRIMARY KEY (id_puesto),
    UNIQUE KEY uk_puestos_puesto (puesto)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE clientes (
    id_cliente INT NOT NULL AUTO_INCREMENT,
    nombres VARCHAR(60) NOT NULL,
    apellidos VARCHAR(60) NOT NULL,
    nit VARCHAR(12) NOT NULL,
    genero BIT(1) NOT NULL DEFAULT b'1',
    telefono VARCHAR(25) NOT NULL,
    correo_electronico VARCHAR(45) DEFAULT NULL,
    fecha_ingreso DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id_cliente),
    UNIQUE KEY uk_clientes_nit (nit),
    KEY idx_clientes_nombre (nombres, apellidos)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE proveedores (
    id_proveedor INT NOT NULL AUTO_INCREMENT,
    proveedor VARCHAR(60) NOT NULL,
    nit VARCHAR(12) NOT NULL,
    direccion VARCHAR(80) NOT NULL,
    telefono VARCHAR(25) NOT NULL,
    PRIMARY KEY (id_proveedor),
    UNIQUE KEY uk_proveedores_nit (nit),
    KEY idx_proveedores_nombre (proveedor)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE empleados (
    id_empleado INT NOT NULL AUTO_INCREMENT,
    nombres VARCHAR(60) NOT NULL,
    apellidos VARCHAR(60) NOT NULL,
    direccion VARCHAR(80) NOT NULL,
    telefono VARCHAR(25) NOT NULL,
    cui VARCHAR(15) NOT NULL,
    genero BIT(1) NOT NULL DEFAULT b'1',
    fecha_nacimiento DATE NOT NULL,
    id_puesto SMALLINT NOT NULL,
    fecha_inicio_labores DATE NOT NULL,
    fecha_ingreso DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id_empleado),
    UNIQUE KEY uk_empleados_cui (cui),
    KEY idx_empleados_puesto (id_puesto),
    KEY idx_empleados_nombre (nombres, apellidos),
    CONSTRAINT fk_empleados_puestos FOREIGN KEY (id_puesto) REFERENCES puestos (id_puesto)
        ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE productos (
    id_producto INT NOT NULL AUTO_INCREMENT,
    producto VARCHAR(50) NOT NULL,
    id_marca SMALLINT NOT NULL,
    descripcion VARCHAR(100) DEFAULT NULL,
    imagen VARCHAR(30) DEFAULT NULL,
    precio_costo DECIMAL(8,2) NOT NULL DEFAULT 0.00,
    precio_venta DECIMAL(8,2) NOT NULL DEFAULT 0.00,
    existencia INT NOT NULL DEFAULT 0,
    fecha_ingreso DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id_producto),
    KEY idx_productos_marca (id_marca),
    KEY idx_productos_nombre (producto),
    CONSTRAINT fk_productos_marcas FOREIGN KEY (id_marca) REFERENCES marcas (id_marca)
        ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE compras (
    id_compra INT NOT NULL AUTO_INCREMENT,
    no_orden_compra INT NOT NULL,
    id_proveedor INT NOT NULL,
    fecha_orden DATE NOT NULL,
    fecha_ingreso DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id_compra),
    UNIQUE KEY uk_compras_no_orden (no_orden_compra),
    KEY idx_compras_proveedor (id_proveedor),
    CONSTRAINT fk_compras_proveedores FOREIGN KEY (id_proveedor) REFERENCES proveedores (id_proveedor)
        ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE compras_detalle (
    id_compra_detalle BIGINT NOT NULL AUTO_INCREMENT,
    id_compra INT NOT NULL,
    id_producto INT NOT NULL,
    cantidad INT NOT NULL,
    precio_costo_unitario DECIMAL(8,2) NOT NULL,
    PRIMARY KEY (id_compra_detalle),
    KEY idx_compras_detalle_compra (id_compra),
    KEY idx_compras_detalle_producto (id_producto),
    CONSTRAINT fk_compras_detalle_compras FOREIGN KEY (id_compra) REFERENCES compras (id_compra)
        ON UPDATE CASCADE ON DELETE CASCADE,
    CONSTRAINT fk_compras_detalle_productos FOREIGN KEY (id_producto) REFERENCES productos (id_producto)
        ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE ventas (
    id_venta INT NOT NULL AUTO_INCREMENT,
    no_factura INT NOT NULL,
    serie CHAR(1) NOT NULL DEFAULT 'A',
    fecha_factura DATE NOT NULL,
    id_cliente INT NOT NULL,
    id_empleado INT NOT NULL,
    fecha_ingreso DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id_venta),
    UNIQUE KEY uk_ventas_factura (serie, no_factura),
    KEY idx_ventas_cliente (id_cliente),
    KEY idx_ventas_empleado (id_empleado),
    CONSTRAINT fk_ventas_clientes FOREIGN KEY (id_cliente) REFERENCES clientes (id_cliente)
        ON UPDATE CASCADE ON DELETE RESTRICT,
    CONSTRAINT fk_ventas_empleados FOREIGN KEY (id_empleado) REFERENCES empleados (id_empleado)
        ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE ventas_detalle (
    id_venta_detalle BIGINT NOT NULL AUTO_INCREMENT,
    id_venta INT NOT NULL,
    id_producto INT NOT NULL,
    cantidad INT NOT NULL,
    precio_unitario DECIMAL(8,2) NOT NULL,
    PRIMARY KEY (id_venta_detalle),
    KEY idx_ventas_detalle_venta (id_venta),
    KEY idx_ventas_detalle_producto (id_producto),
    CONSTRAINT fk_ventas_detalle_ventas FOREIGN KEY (id_venta) REFERENCES ventas (id_venta)
        ON UPDATE CASCADE ON DELETE CASCADE,
    CONSTRAINT fk_ventas_detalle_productos FOREIGN KEY (id_producto) REFERENCES productos (id_producto)
        ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

INSERT INTO marcas (marca) VALUES
('Coca Cola'),('Pepsi'),('Bimbo'),('Diana'),('Malher'),('La Moderna'),('Nestle'),('Kellogg''s'),('Colgate'),('Suli');

INSERT INTO puestos (puesto) VALUES
('Administrador'),('Cajero'),('Vendedor'),('Bodeguero'),('Compras'),('Supervisor');

INSERT INTO clientes (nombres, apellidos, nit, genero, telefono, correo_electronico, fecha_ingreso) VALUES
('Consumidor', 'Final', 'C/F', b'1', '00000000', NULL, NOW()),
('Carlos Enrique', 'Lopez Perez', '1234567-8', b'1', '55551234', 'carlos@example.com', NOW()),
('Maria Fernanda', 'Garcia Soto', '9876543-2', b'0', '55557654', 'maria@example.com', NOW());

INSERT INTO proveedores (proveedor, nit, direccion, telefono) VALUES
('Distribuidora Central', '1111111-1', 'Zona 1, Ciudad de Guatemala', '22223333'),
('Comercial La Economica', '2222222-2', 'Zona 4, Ciudad de Guatemala', '23334444'),
('Proveedora del Norte', '3333333-3', 'Zona 6, Ciudad de Guatemala', '24445555');

INSERT INTO empleados (nombres, apellidos, direccion, telefono, cui, genero, fecha_nacimiento, id_puesto, fecha_inicio_labores, fecha_ingreso) VALUES
('Abraham Isaac', 'Lopez Cifuentes', 'Ciudad de Guatemala', '55550001', '1234567890101', b'1', '1990-05-15', 1, '2024-01-10', NOW()),
('Luis Fernando', 'Perez Gomez', 'Mixco', '55550002', '1234567890102', b'1', '1995-03-20', 2, '2024-02-01', NOW()),
('Andrea Maria', 'Santos Ruiz', 'Villa Nueva', '55550003', '1234567890103', b'0', '1998-08-12', 3, '2024-03-05', NOW());

INSERT INTO productos (producto, id_marca, descripcion, imagen, precio_costo, precio_venta, existencia, fecha_ingreso) VALUES
('Gaseosa Coca Cola 2L', 1, 'Bebida gaseosa 2L', 'coca_2l.jpg', 8.00, 12.00, 50, NOW()),
('Gaseosa Pepsi 2L', 2, 'Bebida gaseosa 2L', 'pepsi_2l.jpg', 7.50, 11.50, 45, NOW()),
('Pan Sandwich Grande', 3, 'Pan blanco', 'pan.jpg', 12.00, 18.00, 35, NOW()),
('Nachos Diana', 4, 'Bolsa de nachos', 'nachos.jpg', 3.00, 5.00, 100, NOW()),
('Corn Flakes Kellogg''s', 8, 'Cereal Corn Flakes', 'cereal.jpg', 18.00, 25.00, 40, NOW());

SELECT 'Base de datos db_supermercado creada con estructura del diagrama' AS mensaje;
