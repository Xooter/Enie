# 18 KB of pure ñ

> ONLY WORK IN LINUX WAYLAND

## Macros?

yep, macros.

## Why?

Because I'm tired of writing ñ

### Write

| nie = ñe |
| nio = ño |
| nia = ña |
| ;; = ñ |

### How to make it work?

```bash
sudo nano /etc/systemd/system/enie.service
```

```bash
[Unit]
Description=Some description
After=network.target

[Service]
ExecStart=/path/to/your/program path/to/your/device
User=root
Type=simple

[Install]
WantedBy=multi-user.target
```

```bash
sudo systemctl daemon-reload
```

```bash
sudo systemctl enable enie.service
```

```bash
sudo systemctl start enie.service
```

### How i know my device?

Usually the keyboard default its /dev/input/event3

You can check with:

```bash
sudo evtest
```

### Benchmark

```bash
--------------------------------------------------------------------------------
  n        time(i)         total(B)   useful-heap(B) extra-heap(B)    stacks(B)
--------------------------------------------------------------------------------
 49      2,868,445           75,288           75,168           120            0
 50      2,868,628           74,560           74,472            88            0
 51      2,869,569           74,632           74,536            96            0
 52      2,869,793           75,288           75,168           120            0
 53      2,870,043           75,360           75,204           156            0
 54      2,870,410           75,360           75,204           156            0
 55      2,870,590           75,288           75,168           120            0
 56      2,870,773           74,560           74,472            88            0
 57      2,871,174           74,632           74,536            96            0

```
