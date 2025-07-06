# SPDX-FileCopyrightText: 2018 Christian Sailer
#
# SPDX-License-Identifier: GPL-3.0-or-later

#!/bin/sh

echo Running unit tests
./qtguiTest/qtguiTest && ./moduleTest/moduleTest --allow-running-no-tests || exit 1
