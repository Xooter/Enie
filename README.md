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
